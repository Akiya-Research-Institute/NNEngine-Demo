// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "PoseEstimator.h"
#include "TextureProcessFunctionLibrary.h"

void UPoseEstimator::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
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

    outputValues.Init(0, 51);
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], outputValues.GetData());
}

bool UPoseEstimator::capture(TArray<uint8>& inputRawImage, TArray<FVector2D>& normalizedJointPositions)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    if (checkConfidence())
    {
        parseToVector2d(normalizedJointPositions);
        return true;
    }
    else
    {
        return false;
    }
}

bool UPoseEstimator::checkConfidence()
{
    int count = 0;
    for (int i = 0; i < 17; i++)
    {
        if (outputValues[i * 3 + 2] > 0.6f) count++;
    }
    return count > 5;
}

void UPoseEstimator::parseToVector2d(TArray<FVector2D>& jointPositions)
{
    jointPositions.Reserve(17);
    for (int i = 0; i < 17; i++)
    {
        jointPositions.Emplace(outputValues[i * 3 + 1], outputValues[i * 3]);
    }
}

void UPoseEstimator::calcNextRoi(const TArray<FVector2D>& jointPositions, const FVector2D& uvScalingFactor, FVector2D& center, FVector2D& orientation, float& size)
{
    const FVector2D pelvis = (jointPositions[11] + jointPositions[12]) * 0.5f / uvScalingFactor;
    const FVector2D nose = jointPositions[0] / uvScalingFactor;

    center = pelvis;
    orientation = pelvis - nose;
    orientation.Normalize();

    const TArray<int> indices = { 0, 9, 10, 15, 16 }; //nose, wrists L&R, feet L&R
    float maxDistSquared = 0;
    for (int i : indices)
    {
        const float distSquared = FVector2D::DistSquared(pelvis, jointPositions[i] / uvScalingFactor);
        if (maxDistSquared < distSquared)
        {
            maxDistSquared = distSquared;
        }
    }
    size = FMath::Sqrt(maxDistSquared);
}