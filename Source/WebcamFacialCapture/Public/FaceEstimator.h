// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "FaceEstimator.generated.h"

USTRUCT(BlueprintType)
/** Positions of facial landmarks */
struct FFaceMesh
{
	GENERATED_BODY()

public:
	/** Calculate positions of the eyes from output of the ML model */
	void calculateEyePositions()
	{
		const FVector eyeR = (mesh[159] + mesh[145] + mesh[158] + mesh[153]) * 0.25f;
		const FVector eyeL = (mesh[386] + mesh[374] + mesh[385] + mesh[380]) * 0.25f;
		eyePositionR = FVector2D(eyeR.X, eyeR.Y);
		eyePositionL = FVector2D(eyeL.X, eyeL.Y);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture") 
		/** Facial landmarks output from the ML model. See "Facial_landmarks_visualization.png" */
		TArray<FVector> mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture") 
		/** Position of the right eye calculated from 'mesh' */
		FVector2D eyePositionR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture") 
		/** Position of the left eye calculated from 'mesh' */
		FVector2D eyePositionL;
};

UCLASS(Blueprintable)
/** Manages a OnnxModel of BlazeFace Mesh from [MediaPipe Face Mesh](https://google.github.io/mediapipe/solutions/face_mesh.html) */
class WEBCAMFACIALCAPTURE_API UFaceEstimator : public UObject
{
	GENERATED_BODY()

protected:
	/** OnnxModel of BlazeFace Mesh */
	OnnxModel* onnxModel;
	/** Buffer for the output mesh */
	FFaceMesh faceMesh;
	/** Buffer for the output confidence */
	float confidence;

public:
	UFUNCTION(BlueprintCallable, Category = "Facial Capture") 
		/**
		* Open ONNX model and initialize it.
		* @param onnxFilePath Path of the .onnx file
		* @param onnxProvider Execution provider used for this model
		* @param gpuDeviceId Which GPU to be used
		*/
		void init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId = 0);

	UFUNCTION(BlueprintCallable, Category = "Facial Capture") 
		/** 
		* Estimates the facial landmarks from image
		* @param inputRawImage Input image as an array of bytes whose length is H(192) x W(192) x 3(BGR).
		* @param mesh Facial landmarks estimated from the image. The positions are in the image space.
		* @return Whether the estimation successed
		*/
		bool capture(UPARAM(ref) TArray<uint8>& inputRawImage, FFaceMesh& mesh);
};
