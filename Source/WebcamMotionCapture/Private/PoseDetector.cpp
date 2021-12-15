// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "PoseDetector.h"

void UPoseDetector::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
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

    outputPoints.Init(0.0f, 12);
    outputConfidence.Init(0.0f, 1);

    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], outputPoints.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[1], outputConfidence.GetData());
}

bool UPoseDetector::detect(TArray<uint8>& inputRawImage, FVector2D& normalizedPelvisPosition, FVector2D& normalizedHeadPosition)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    if (outputConfidence[0] > 0.5f)
    {
        normalizedPelvisPosition = FVector2D(outputPoints[4], outputPoints[5]);
        normalizedHeadPosition = FVector2D(outputPoints[10], outputPoints[11]);
        return true;
    }
    else
    {
        return false;
    }
}
