// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "FaceEstimatorAttention.h"

void UFaceEstimatorAttention::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
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

    faceMeshAttention.face.Init(FVector(), 468);
    faceMeshAttention.eyeL.Init(FVector2D(), 71);
    faceMeshAttention.eyeR.Init(FVector2D(), 71);
    faceMeshAttention.irisL.Init(FVector2D(), 5);
    faceMeshAttention.irisR.Init(FVector2D(), 5);
    faceMeshAttention.lips.Init(FVector2D(), 80);
    faceMeshAttention.face_raw.Init(0.0f, 468*3);
    faceMeshAttention.eyeL_raw.Init(0.0f, 71*2);
    faceMeshAttention.eyeR_raw.Init(0.0f, 71*2);
    faceMeshAttention.irisL_raw.Init(0.0f, 5*2);
    faceMeshAttention.irisR_raw.Init(0.0f, 5*2);
    faceMeshAttention.lips_raw.Init(0.0f, 80*2);
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], &faceMeshAttention.confidence);
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[1], faceMeshAttention.eyeL_raw.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[2], faceMeshAttention.irisL_raw.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[3], faceMeshAttention.lips_raw.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[4], faceMeshAttention.face_raw.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[5], faceMeshAttention.eyeR_raw.GetData());
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[6], faceMeshAttention.irisR_raw.GetData());
}

bool UFaceEstimatorAttention::capture(TArray<uint8>& inputRawImage, FFaceMeshAttention& outputMesh)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    faceMeshAttention.parseToDoubleVectors();
    outputMesh = faceMeshAttention;
    return faceMeshAttention.confidence > 0.9f;
}

void UFaceEstimatorAttention::calcNextRoiDiff(FVector2D& center, FVector2D& orientation, float& size)
{
    // calc min and max of x and y
    FVector2D minLandmark = FVector2D(faceMeshAttention.face[0]);
    FVector2D maxLandmark = FVector2D(faceMeshAttention.face[0]);

    for (FVector landmark : faceMeshAttention.face)
    {
        minLandmark = FVector2D(
            FMath::Min(minLandmark.X, landmark.X),
            FMath::Min(minLandmark.Y, landmark.Y)
        );
        maxLandmark = FVector2D(
            FMath::Max(maxLandmark.X, landmark.X),
            FMath::Max(maxLandmark.Y, landmark.Y)
        );
    }

    center = (minLandmark + maxLandmark) * 0.5f;

    orientation = FVector2D(
        - faceMeshAttention.eyeR[0].Y + faceMeshAttention.eyeL[0].Y,
        faceMeshAttention.eyeR[0].X - faceMeshAttention.eyeL[0].X
    );

    orientation.Normalize();

    const FVector2D width = maxLandmark - minLandmark;
    size = FMath::Max(width.X, width.Y) * 0.5f;
}