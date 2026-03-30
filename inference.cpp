#include "inference.h"
#include <regex>

#define benchmark
#define ELOG

float clamp(float val, float min, float max)
{
    return val > min ? (val < max ? val : max) : min;
}

/////////////////////////////////////////////////////////////////////////////////////////////
#if 1
struct PreprocessResult 
{
    cv::Mat image;
    float ratio;
    float dw, dh;
};

PreprocessResult preprocess_keep_aspect(const cv::Mat& image, int inputWidth = 640, int inputHeight = 640) {
    PreprocessResult result;

    int original_height = image.rows;
    int original_width = image.cols;

    // 计算缩放比例（与Python版本一致）
    float r = std::min(static_cast<float>(inputWidth) / original_width,
        static_cast<float>(inputHeight) / original_height);
    int new_w = static_cast<int>(original_width * r);
    int new_h = static_cast<int>(original_height * r);

    // 调整大小
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(new_w, new_h));

    // 创建画布并填充
    cv::Mat canvas = cv::Mat::zeros(inputHeight, inputWidth, CV_8UC3);
    canvas.setTo(cv::Scalar(114, 114, 114));

    // 计算填充偏移量
    result.dw = (inputWidth - new_w) / 2.0f;
    result.dh = (inputHeight - new_h) / 2.0f;
    result.ratio = r;

    int top = static_cast<int>(std::round(result.dh - 0.1f));
    int left = static_cast<int>(std::round(result.dw - 0.1f));

    // 将调整大小后的图像放在画布中央
    cv::Mat roi = canvas(cv::Rect(left, top, new_w, new_h));
    resized.copyTo(roi);

    // 转换颜色通道 BGR -> RGB
    //cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);

    // 归一化
    canvas.convertTo(result.image, CV_32F, 1.0/255.0);

    return result;
}

struct DetectionResult {
    bool hasKeypoints;
    cv::Rect_<float> bbox;
    std::vector<Keypoint> keypoints;
};

DetectionResult optimizeDetectionResult(const DetectionResult& input,
    const cv::Size& original_size,
    const cv::Size& model_input_size,
    float ratio, float pad_x, float pad_y,
    float margin = 0.0f,
    bool verbose = false) {
    DetectionResult result = input;

    if (!result.hasKeypoints || result.keypoints.empty()) {
        if (verbose) {
            std::cout << "No keypoints to optimize" << std::endl;
        }
        return result;
    }

    // 将边界框坐标从模型输入尺寸映射回原图尺寸
    // 注意：这里需要减去填充并除以缩放比例
    result.bbox.x = (result.bbox.x - pad_x) * ratio;
    result.bbox.y = (result.bbox.y - pad_y) * ratio;
    result.bbox.width = result.bbox.width * ratio;
    result.bbox.height = result.bbox.height * ratio;

    // 确保边界框在原图范围内
    result.bbox.x = std::max(0.0f, std::min(result.bbox.x, static_cast<float>(original_size.width - 1)));
    result.bbox.y = std::max(0.0f, std::min(result.bbox.y, static_cast<float>(original_size.height - 1)));
    result.bbox.width = std::max(0.0f, std::min(result.bbox.width, static_cast<float>(original_size.width - result.bbox.x)));
    result.bbox.height = std::max(0.0f, std::min(result.bbox.height, static_cast<float>(original_size.height - result.bbox.y)));

    int original_count = result.keypoints.size();
    std::vector<Keypoint> filtered_keypoints;

    for (const auto& kp : result.keypoints) {
        // 将关键点坐标从模型输入尺寸映射回原图尺寸
        // 同样需要减去填充并除以缩放比例
        float mapped_x = (kp.position.x - pad_x) * ratio;
        float mapped_y = (kp.position.y - pad_y) * ratio;

        // 确保关键点坐标在原图范围内
        mapped_x = std::max(0.0f, std::min(mapped_x, static_cast<float>(original_size.width - 1)));
        mapped_y = std::max(0.0f, std::min(mapped_y, static_cast<float>(original_size.height - 1)));

        // 检查映射后的关键点是否在映射后的检测框内
        if (mapped_x >= result.bbox.x - margin &&
            mapped_x <= result.bbox.x + result.bbox.width + margin &&
            mapped_y >= result.bbox.y - margin &&
            mapped_y <= result.bbox.y + result.bbox.height + margin) {
            // 保存映射后的坐标
            filtered_keypoints.emplace_back(mapped_x, mapped_y, kp.conf);
        }
    }

    result.keypoints = filtered_keypoints;

    if (result.keypoints.empty()) {
        result.hasKeypoints = false;
    }

    if (verbose) {
        std::cout << "Keypoint optimization: " << original_count << " -> "
            << result.keypoints.size() << " keypoints" << std::endl;
        std::cout << "Bounding box after mapping: " << result.bbox << std::endl;
        std::cout << "Mapping parameters - ratio: " << ratio
            << ", pad_x: " << pad_x << ", pad_y: " << pad_y << std::endl;
    }

    return result;
}

void processFrame(cv::Mat& frame, cv::dnn::Net& net, float confThreshold, float nmsThreshold,
    int inputWidth, int inputHeight, int numKeypoints, bool& hasKeypoints, cv::Rect_<float>& out_bbox,
    std::vector<Keypoint>& out_keyps) {

    cv::Size original_size = cv::Size(frame.cols, frame.rows);
    cv::Size model_size = cv::Size(640, 640);
    float ratio = std::max(frame.rows / float(inputHeight), frame.cols / float(inputWidth));
    float pad_x = std::abs(inputWidth - frame.cols / ratio) / 2.0;
    float pad_y = std::abs(inputHeight - frame.rows / ratio) / 2.0;
    //cv::Mat frame_1 = frame.clone();
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();
    PreprocessResult pr = preprocess_keep_aspect(frame);
    frame = pr.image;
    // 创建输入blob
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0, cv::Size(inputWidth, inputHeight), cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    // 前向推理
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    // 处理输出
    if (outputs.empty()) {
        std::cout << "No outputs from network!" << std::endl;
        return;
    }

    const int channels = outputs[0].size[2];
    const int anchors = outputs[0].size[1];

    // 重塑输出格式
    outputs[0] = outputs[0].reshape(1, anchors);
    cv::Mat output1 = outputs[0].t();

    std::vector<cv::Rect> bboxList;
    std::vector<float> scoreList;
    std::vector<int> indicesList;
    std::vector<std::vector<Keypoint>> kpList;

    // 解析每个检测结果
    for (int i = 0; i < channels; i++) {
        auto row_ptr = output1.row(i).ptr<float>();
        auto bbox_ptr = row_ptr;
        auto score_ptr = row_ptr + 4;
        auto kp_ptr = row_ptr + 5;

        float score = *score_ptr;
        if (score > modelScoreThreshold) {
            float x = *bbox_ptr++;
            float y = *bbox_ptr++;
            float w = *bbox_ptr++;
            float h = *bbox_ptr;

            // 转换边界框坐标
            float x0 = clamp((x - 0.5f * w) * 1.0F, 0.f, float(modelShape.width));
            float y0 = clamp((y - 0.5f * h) * 1.0F, 0.f, float(modelShape.height));
            float x1 = clamp((x + 0.5f * w) * 1.0F, 0.f, float(modelShape.width));
            float y1 = clamp((y + 0.5f * h) * 1.0F, 0.f, float(modelShape.height));

            cv::Rect_<float> bbox;
            bbox.x = x0;
            bbox.y = y0;
            bbox.width = x1 - x0;
            bbox.height = y1 - y0;

            // 解析关键点
            std::vector<Keypoint> kps;
            for (int k = 0; k < numKeypoints; k++) {
                float kps_x = (*(kp_ptr + 3 * k));
                float kps_y = (*(kp_ptr + 3 * k + 1));
                float kps_s = *(kp_ptr + 3 * k + 2);

                kps_x = clamp(kps_x * 1.0F, 0.f, float(modelShape.width));
                kps_y = clamp(kps_y * 1.0F, 0.f, float(modelShape.height));
                kps.emplace_back(kps_x, kps_y, kps_s);

            }

            bboxList.push_back(bbox);
            scoreList.push_back(score);
            kpList.push_back(kps);
        }
    }

    std::cout << "Found " << bboxList.size() << " potential detections" << std::endl;

    // 应用NMS
    if (!bboxList.empty() && !scoreList.empty()) {
        cv::dnn::NMSBoxes(bboxList, scoreList, confThreshold, nmsThreshold, indicesList);

        std::cout << "After NMS: " << indicesList.size() << " detections" << std::endl;

        cv::Rect_<float> bbox;
        std::vector<Keypoint> keyps;
        bool hasKeys = false;
        if (!indicesList.empty()) {
            int best_idx = indicesList[0];
            bbox = bboxList[best_idx];
            keyps = kpList[best_idx];
            hasKeys = true;
        }
        DetectionResult tmp;
        tmp.hasKeypoints = hasKeys;
        tmp.bbox = bbox;
        tmp.keypoints = keyps;
        //for (int kk = 0; kk = keyps.size(); kk++) {
        //    std::cout << "------------------" << keyps[kk].position << "-----------------------------" << std::endl;
       // }
        DetectionResult optimizedResults = optimizeDetectionResult(tmp, original_size, model_size, ratio, pad_x, pad_y);
        hasKeypoints = optimizedResults.hasKeypoints;
        out_bbox = optimizedResults.bbox;
        out_keyps = optimizedResults.keypoints;
        //for (int kk = 0; kk = out_keyps.size(); kk++) {
        //    std::cout << "------------------" << out_keyps[kk].position << "-----------------------------" << std::endl;
        //}
    /////////////////////// just for visulization /////v////////////////////////////////////////////////
     //cv::Rect_<float> vbbox = bbox;
     //std::vector<Keypoint> vkeyps = keyps;
     //cv::rectangle(frame_1, vbbox, cv::Scalar(0, 255, 255));
     //for (int k = 0; k < vkeyps.size(); k++) {
         //std::cout << "keyps[" << k << "]: " << vkeyps[k].position.x << ", " << vkeyps[k].position.y << ", " << vkeyps[k].conf << std::endl;
         //if (vkeyps[k].conf > 0.25) {
             //cv::Point2f kp_point(vkeyps[k].position.x, vkeyps[k].position.y);
             //cv::circle(frame_1, kp_point, 5, cv::Scalar(0, 255, 0), -1);
         //}
     //}
     //cv::imshow("keyps", frame_1);
     //cv::waitKey(0);
    }
}

#endif

void processFrameOld(cv::Mat& frame, cv::dnn::Net& net,
    float confThreshold, float nmsThreshold,
    int inputWidth, int inputHeight, int numKeypoints, bool& hasKeypoints, cv::Rect_<float>& out_bbox, std::vector<Keypoint>& out_keyps) {
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 创建输入blob
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0,
        cv::Size(inputWidth, inputHeight),
        cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    // 前向推理
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    ///////////////////////////////////////////////////////////////////////////////////////////
    const int channels = outputs[0].size[2];
    const int anchors = outputs[0].size[1];
    outputs[0] = outputs[0].reshape(1, anchors);
    cv::Mat output1 = outputs[0].t();

    std::vector<cv::Rect> bboxList;
    std::vector<float> scoreList;
    std::vector<int> indicesList;
    std::vector<std::vector<Keypoint>> kpList;

    for (int i = 0; i < channels; i++)
    {
        auto row_ptr = output1.row(i).ptr<float>();
        auto bbox_ptr = row_ptr;
        auto score_ptr = row_ptr + 4;
        auto kp_ptr = row_ptr + 5;

        float score = *score_ptr;
        if (score > modelScoreThreshold) {
            float x = *bbox_ptr++;
            float y = *bbox_ptr++;
            float w = *bbox_ptr++;
            float h = *bbox_ptr;

            float x0 = clamp((x - 0.5f * w) * 1.0F, 0.f, float(modelShape.width));
            float y0 = clamp((y - 0.5f * h) * 1.0F, 0.f, float(modelShape.height));
            float x1 = clamp((x + 0.5f * w) * 1.0F, 0.f, float(modelShape.width));
            float y1 = clamp((y + 0.5f * h) * 1.0F, 0.f, float(modelShape.height));

            cv::Rect_<float> bbox;
            bbox.x = x0;
            bbox.y = y0;
            bbox.width = x1 - x0;
            bbox.height = y1 - y0;

            std::vector<Keypoint> kps;
            for (int k = 0; k < 15; k++) {
                float kps_x = (*(kp_ptr + 3 * k));
                float kps_y = (*(kp_ptr + 3 * k + 1));
                float kps_s = *(kp_ptr + 3 * k + 2);
                if (kps_s > 0.05) {
                    kps_x = clamp(kps_x * 1.0F, 0.f, float(modelShape.width));
                    kps_y = clamp(kps_y * 1.0F, 0.f, float(modelShape.height));
                    kps.emplace_back(kps_x, kps_y, kps_s);
                }
            }

            bboxList.push_back(bbox);
            scoreList.push_back(score);
            kpList.push_back(kps);
        }

    }
    std::cout << "bboxList size: " << bboxList.size() << std::endl;
    std::cout << "scoreList size: " << scoreList.size() << std::endl;
    std::cout << "kpList size: " << kpList.size() << std::endl;
    if (bboxList.size() == 0 || scoreList.size() == 0 || kpList.size() == 0) {
        std::cout << "No valid detections found." << std::endl;
        hasKeypoints = false; // 没有有效的检测结
    }
    else {
        hasKeypoints = true; // 有有效的检测结
        cv::dnn::NMSBoxes(bboxList, scoreList, modelScoreThreshold, modelNMSThreshold, indicesList);
        cv::Rect_<float> bbox;
        std::cout << "indicesList size: " << indicesList.size() << std::endl;
        std::cout << "indicesList value: " << indicesList[0] << std::endl;
        std::vector<Keypoint> keyps;
        if (indicesList.size() > 0) {
            bbox = bboxList[indicesList[0]];
            keyps = kpList[indicesList[0]];
            out_bbox = bbox;
            out_keyps = keyps;
            std::cout << "bbox: " << bbox << std::endl;
            std::cout << "keyps size: " << keyps.size() << std::endl;
            std::cout << "keyps[0]: " << keyps[0].position.x << ", " << keyps[0].position.y << ", " << keyps[0].conf << std::endl;
            std::cout << "keyps[1]: " << keyps[1].position.x << ", " << keyps[1].position.y << ", " << keyps[1].conf << std::endl;
        }
        else {
            std::cout << "No valid indices found." << std::endl;
            // continue; // 如果没有有效的索引，跳过当前循环
        }
        cv::Mat frame_1 = frame.clone();
        cv::rectangle(frame_1, bbox, cv::Scalar(0, 255, 255));
        for (int k = 0; k < keyps.size(); k++) {
            std::cout << "keyps[" << k << "]: " << keyps[k].position.x << ", " << keyps[k].position.y << ", " << keyps[k].conf << std::endl;
            if (keyps[k].conf > 0.05) {
                cv::Point2f kp_point(keyps[k].position.x, keyps[k].position.y);
                cv::circle(frame_1, kp_point, 5, cv::Scalar(0, 255, 0), -1);
            }
        }
        //cv::imshow("bbox", frame_1);
        //cv::waitKey(0);
    }
}

#if 0
DCSP_CORE::DCSP_CORE()
{
}


DCSP_CORE::~DCSP_CORE()
{
	delete session;
}

//template<typename T>
char* BlobFromImage(cv::Mat& iImg, float* &iBlob)
{
	int channels = iImg.channels();
	int imgHeight = iImg.rows;
	int imgWidth = iImg.cols;

	for (int c = 0; c < channels; c++)
	{
		for (int h = 0; h < imgHeight; h++)
		{
			for (int w = 0; w < imgWidth; w++)
			{
                iBlob[c * imgWidth * imgHeight + h * imgWidth + w] = std::remove_pointer<float*>::type((iImg.at<cv::Vec3b>(h, w)[c]) / 255.0f);
			}
		}
	}
	return RET_OK;
}


char* PostProcess(cv::Mat& iImg, std::vector<int> iImgSize, cv::Mat& oImg)
{
	cv::Mat img = iImg.clone();
	cv::resize(iImg, oImg, cv::Size(iImgSize.at(0), iImgSize.at(1)));
	if (img.channels() == 1)
	{
		cv::cvtColor(oImg, oImg, cv::COLOR_GRAY2BGR);
	}
	cv::cvtColor(oImg, oImg, cv::COLOR_BGR2RGB);
	return RET_OK;
}


char* DCSP_CORE::CreateSession(DCSP_INIT_PARAM &iParams)
{
    char* Ret = RET_OK;
	std::regex pattern("[\u4e00-\u9fa5]");
	bool result = std::regex_search(iParams.ModelPath, pattern);
    if (result)
	{
        //Ret = "[DCSP_ONNX]:model path error.change your model path without chinese characters.";
		std::cout << Ret << std::endl;
		return Ret;
	}
	try
	{
		rectConfidenceThreshold = iParams.RectConfidenceThreshold;
		iouThreshold = iParams.iouThreshold;
		imgSize = iParams.imgSize;
		modelType = iParams.ModelType;
		env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "Yolo");
		Ort::SessionOptions sessionOption;
		if (iParams.CudaEnable)
		{
			cudaEnable = iParams.CudaEnable;
			OrtCUDAProviderOptions cudaOption;
			cudaOption.device_id = 0;
			sessionOption.AppendExecutionProvider_CUDA(cudaOption);
			//OrtOpenVINOProviderOptions ovOption;
			//sessionOption.AppendExecutionProvider_OpenVINO(ovOption);
		}
		sessionOption.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
		sessionOption.SetIntraOpNumThreads(iParams.IntraOpNumThreads);
		sessionOption.SetLogSeverityLevel(iParams.LogSeverityLevel);
#ifdef unix
        session = new Ort::Session(env, iParams.ModelPath.c_str(), sessionOption);
#else
        int ModelPathSize = MultiByteToWideChar(CP_UTF8, 0, iParams.ModelPath.c_str(), static_cast<int>(iParams.ModelPath.length()), nullptr, 0);
        wchar_t* wide_cstr = new wchar_t[ModelPathSize + 1];
		MultiByteToWideChar(CP_UTF8, 0, iParams.ModelPath.c_str(), static_cast<int>(iParams.ModelPath.length()), wide_cstr, ModelPathSize);
		wide_cstr[ModelPathSize] = L'\0';
        const wchar_t* modelPath = wide_cstr;
		session = new Ort::Session(env, modelPath, sessionOption);
#endif

		Ort::AllocatorWithDefaultOptions allocator;
		size_t inputNodesNum = session->GetInputCount();
		for (size_t i = 0; i < inputNodesNum; i++)
		{
			Ort::AllocatedStringPtr input_node_name = session->GetInputNameAllocated(i, allocator);
			char* temp_buf = new char[50];
			strcpy(temp_buf, input_node_name.get());
			inputNodeNames.push_back(temp_buf);
		}

		size_t OutputNodesNum = session->GetOutputCount();
		for (size_t i = 0; i < OutputNodesNum; i++)
		{
			Ort::AllocatedStringPtr output_node_name = session->GetOutputNameAllocated(i, allocator);
			char* temp_buf = new char[10];
			strcpy(temp_buf, output_node_name.get());
			outputNodeNames.push_back(temp_buf);
		}
		options = Ort::RunOptions{ nullptr };
        //@@@WarmUpSession();
        std::cout << OrtGetApiBase()->GetVersionString() << std::endl;;
		Ret = RET_OK;
		return Ret;
	}
	catch (const std::exception& e)
	{
		const char* str1 = "[DCSP_ONNX]:";
		const char* str2 = e.what();
		std::string result = std::string(str1) + std::string(str2);
		char* merged = new char[result.length() + 1];
		std::strcpy(merged, result.c_str());
		std::cout << merged << std::endl;
		delete[] merged;
		//return merged;
        return RET_OK;   //@@@"[DCSP_ONNX]:Create session failed.";
	}

}


char* DCSP_CORE::RunSession(cv::Mat &iImg, std::vector<DCSP_RESULT>& oResult)
{
#ifdef benchmark
	clock_t starttime_1 = clock();
#endif // benchmark

	char* Ret = RET_OK;
	cv::Mat processedImg;
	PostProcess(iImg, imgSize, processedImg);
	if (modelType < 4)
	{
		float* blob = new float[processedImg.total() * 3];
		BlobFromImage(processedImg, blob);
        std::vector<int64_t> inputNodeDims = { 1, 3, imgSize.at(0), imgSize.at(1)};
		TensorProcess(starttime_1, iImg, blob, inputNodeDims, oResult);
	}

	return Ret;
}

struct det{
    float x;
    float y;
    float w;
    float h;
    float th;
};

struct det_point{
    float x;
    float y;
    float th;
};

struct back_data{
    int i;
    det d;
    std::vector<det_point> p;
    float tth = 0;
    float tth2;
};

struct back_result{
    std::vector<back_data> br;
};

//template<typename N>
char* DCSP_CORE::TensorProcess(clock_t& starttime_1, cv::Mat& iImg, float* &blob, std::vector<int64_t>& inputNodeDims,  std::vector<DCSP_RESULT>& oResult)
{
    Ort::Value inputTensor = Ort::Value::CreateTensor<std::remove_pointer<float*>::type>(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU), blob, 3 * imgSize.at(0) * imgSize.at(1), inputNodeDims.data(), inputNodeDims.size());
#ifdef benchmark
	clock_t starttime_2 = clock();
#endif // benchmark
    std::vector<Ort::Value> outputTensor = session->Run(options, inputNodeNames.data(), &inputTensor, 1, outputNodeNames.data(), outputNodeNames.size());
#ifdef benchmark
	clock_t starttime_3 = clock();
#endif // benchmark
	Ort::TypeInfo typeInfo = outputTensor.front().GetTypeInfo();
	auto tensor_info = typeInfo.GetTensorTypeAndShapeInfo();
	std::vector<int64_t>outputNodeDims = tensor_info.GetShape();
    std::remove_pointer<float*>::type* output = outputTensor.front().GetTensorMutableData<std::remove_pointer<float*>::type>();
	delete blob;
	switch (modelType)
	{
    case YOLO_ORIGIN_V8:
    {
        int strideNum = outputNodeDims[2];
        int signalResultNum = outputNodeDims[1];
        std::vector<int> class_ids;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
        cv::Mat rowData(signalResultNum, strideNum, CV_32F, output);
        rowData = rowData.t();

        //cv::imshow("rowData", rowData);
        float* data = (float*)rowData.data;

        float x_factor = iImg.cols / 640.;
        float y_factor = iImg.rows / 640.;
        for (int i = 0; i < strideNum; ++i)
        {
            float* classesScores = data + 4;
            cv::Mat scores(1, classesNum, CV_32FC1, classesScores);
            cv::Point class_id;
            double maxClassScore;
            cv::minMaxLoc(scores, nullptr, &maxClassScore, nullptr, &class_id);
            if (maxClassScore > rectConfidenceThreshold)
            {
                confidences.push_back(maxClassScore);
                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);

                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
            data += signalResultNum;
        }

        std::vector<int> nmsResult;
        cv::dnn::NMSBoxes(boxes, confidences, rectConfidenceThreshold, iouThreshold, nmsResult);
        for (int i = 0; i < nmsResult.size(); ++i)
        {
            int idx = nmsResult[i];
            DCSP_RESULT result;
            result.classId = class_ids[idx];
            result.confidence = confidences[idx];
            result.box = boxes[idx];
            oResult.push_back(result);
        }


#ifdef benchmark
        clock_t starttime_4 = clock();
        double pre_process_time = (double)(starttime_2 - starttime_1) / CLOCKS_PER_SEC * 1000;
        double process_time = (double)(starttime_3 - starttime_2) / CLOCKS_PER_SEC * 1000;
        double post_process_time = (double)(starttime_4 - starttime_3) / CLOCKS_PER_SEC * 1000;
        if (cudaEnable)
        {
            std::cout << "[DCSP_ONNX(CUDA)]: " << pre_process_time << "ms pre-process, " << process_time << "ms inference, " << post_process_time << "ms post-process." << std::endl;
        }
        else
        {
            std::cout << "[DCSP_ONNX(CPU)]: " << pre_process_time << "ms pre-process, " << process_time << "ms inference, " << post_process_time << "ms post-process." << std::endl;
        }
#endif // benchmark

        break;
    }
    case YOLO_POSE_V8:
    {
        int strideNum = outputNodeDims[2];
        int signalResultNum = outputNodeDims[1];
        std::vector<int> class_ids;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
        cv::Mat rowData(signalResultNum, strideNum, CV_32F, output);
        rowData = rowData.t();

        //cv::imshow("rowData", rowData);
        float* data = (float*)rowData.data;

        std::cout << std::endl;
        float x_factor = iImg.cols / 640.0f;
        float y_factor = iImg.rows / 640.0f;
        back_data bd_max;
        for (int i = 0; i < strideNum; ++i)
        {
            back_data bd;

           #if 0
            std::cout << std::endl;
            std::cout << std::endl;

            printf("%d: ", i);
            printf("%5.2f,", double(data[0]));
            printf("%5.2f,", double(data[1]));
            printf("%5.2f,", double(data[2]));
            printf("%5.2f,", double(data[3]));
            printf("%5.2f;  ", double(data[4]));
           #endif
            bd.i = i;
            bd.d = {data[0], data[1], data[2], data[3], data[4]};
            std::list<double> tmp_score;
            for (int var = 5; var < 47; var+=3) {
           #if 0
               printf("%5.2f,", double(data[var]));
               printf("%5.2f,", double(data[var + 1]));
               printf("%5.2f; ", double(data[var + 2]));
           #endif
               det_point dp = {data[var], data[var + 1], data[var + 2]};
               bd.p.push_back(dp);
               bd.tth += dp.th;
            }
            bd_max = bd.tth > bd_max.tth ? bd : bd_max;
            data += signalResultNum;
        }

#if 0
        for (uint i = 0; i < 30; ++i) {
            cv::Mat imgA = iImg.clone();
            cv::resize(imgA, imgA, cv::Size(640, 640));
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << i << ", " << br.br[i].i  << ", " << br.br[i].tth << ": " ;
            for (uint var = 0; var < 14; ++var) {
                std::cout<< br.br[i].p[var].th << ", "
                          << br.br[i].p[var].x << ", "
                          << br.br[i].p[var].y << "; ";
                cv::circle(imgA, cv::Point2f(br.br[i].p[var].x, br.br[i].p[var].y), 3, cv::Scalar(128, 5 * 5, 255), 3);
                cv::putText(imgA, std::to_string(double(br.br[i].p[var].th)), cv::Point2f(br.br[i].p[var].x + 11, br.br[i].p[var].y + 11),
                        1, 0.3, cv::Scalar(0, 0, 128));
            }
            cv::imshow("imgA", imgA);
            cv::waitKey(3000);
        }
#endif
        for (uint i = 0; i < 14; ++i) {
            float x = bd_max.p[i].x;
            float y = bd_max.p[i].y;
            float w = 4;
            float h = 4;

            int left = int((x - 0.5f * w) * x_factor);
            int top = int((y - 0.5f * h) * y_factor);

            int width = int(w * x_factor);
            int height = int(h * y_factor);

            DCSP_RESULT result;
            result.classId = 0;
            result.confidence = bd_max.p[i].th;
            result.box = cv::Rect(left, top, width, height);

            oResult.push_back(result);
        }


#ifdef benchmark
        clock_t starttime_4 = clock();
        double pre_process_time = (double)(starttime_2 - starttime_1) / CLOCKS_PER_SEC * 1000;
        double process_time = (double)(starttime_3 - starttime_2) / CLOCKS_PER_SEC * 1000;
        double post_process_time = (double)(starttime_4 - starttime_3) / CLOCKS_PER_SEC * 1000;
        if (cudaEnable)
        {
            std::cout << "[DCSP_ONNX(CUDA)]: " << pre_process_time << "ms pre-process, " << process_time << "ms inference, " << post_process_time << "ms post-process." << std::endl;
        }
        else
        {
            std::cout << "[DCSP_ONNX(CPU)]: " << pre_process_time << "ms pre-process, " << process_time << "ms inference, " << post_process_time << "ms post-process." << std::endl;
        }
#endif // benchmark

        break;
    }
	}
	char* Ret = RET_OK;
	return Ret;
}


char* DCSP_CORE::WarmUpSession()
{
	clock_t starttime_1 = clock();
	char* Ret = RET_OK;
	cv::Mat iImg = cv::Mat(cv::Size(imgSize.at(0), imgSize.at(1)), CV_8UC3);
	cv::Mat processedImg;
	PostProcess(iImg, imgSize, processedImg);
	if (modelType < 4)
	{
		float* blob = new float[iImg.total() * 3];
		BlobFromImage(processedImg, blob);
		std::vector<int64_t> YOLO_input_node_dims = { 1,3,imgSize.at(0),imgSize.at(1) };
		Ort::Value input_tensor = Ort::Value::CreateTensor<float>(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU), blob, 3 * imgSize.at(0) * imgSize.at(1), YOLO_input_node_dims.data(), YOLO_input_node_dims.size());
		auto output_tensors = session->Run(options, inputNodeNames.data(), &input_tensor, 1, outputNodeNames.data(), outputNodeNames.size());
		delete[] blob;
		clock_t starttime_4 = clock();
		double post_process_time = (double)(starttime_4 - starttime_1) / CLOCKS_PER_SEC * 1000;
		if (cudaEnable)
		{
			std::cout << "[DCSP_ONNX(CUDA)]: " << "Cuda warm-up cost " << post_process_time << " ms. " << std::endl;
		}
	}

	return Ret;
}

#endif

