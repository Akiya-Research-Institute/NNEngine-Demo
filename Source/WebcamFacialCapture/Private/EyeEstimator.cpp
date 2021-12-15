// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "EyeEstimator.h"

#include "opencv2/core/core.hpp"
#include <opencv2/imgproc.hpp>

void UEyeEstimator::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
{
    try
    {
        if (onnxModel) delete onnxModel;
        onnxModel = new OnnxModel(onnxFilePath, onnxProvider, gpuDeviceId);
    }
    catch (Ort::Exception e)
    {
        if (onnxModel) delete onnxModel;
        onnxModel = new OnnxModel(onnxFilePath, EOnnxProvider::CPU, 0);
    }

    outputEyePoints.Init(FVector(), 71);
    outputIrisPoints.Init(FVector(), 5);

    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], outputEyePoints.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[1], outputIrisPoints.GetData());
}

void UEyeEstimator::crop(const TArray<uint8>& faceRawImage, const FVector2D& center, TArray<uint8>& eyeRawImage, const int size)
{
    eyeRawImage.Init(0, 64*64*3);
    cv::Mat inImage(cv::Size({ 192, 192 }), CV_8UC3, (void*)faceRawImage.GetData());
    cv::Mat outImage(cv::Size({ 64, 64 }), CV_8UC3, (void*)eyeRawImage.GetData());
    const cv::Rect eyeBbox(center.X - size, center.Y - size, size * 2, size * 2);
    const cv::Mat eyeImg = inImage(eyeBbox);
    cv::resize(eyeImg, outImage, cv::Size(64, 64));
}

void UEyeEstimator::capture(TArray<uint8>& inputRawImage, TArray<FVector>& out_eyePoints, TArray<FVector>& out_irisPoints)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    out_eyePoints = outputEyePoints;
    out_irisPoints = outputIrisPoints;
}