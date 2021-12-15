// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "FaceEstimator.h"

void UFaceEstimator::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
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

    faceMesh.mesh.Init(FVector(), 468);
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], faceMesh.mesh.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[1], &confidence);
}

bool UFaceEstimator::capture(TArray<uint8>& inputRawImage, FFaceMesh& mesh)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    if (confidence > 0.98f)
    {
        faceMesh.calculateEyePositions();
        mesh = faceMesh;
        return true;
    }
    else
    {
        return false;
    }
}
