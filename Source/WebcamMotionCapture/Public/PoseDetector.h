// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "PoseDetector.generated.h"

UCLASS(Blueprintable)
/** Manages a OnnxModel of BlazePose Detection from [MediaPipe Pose](https://google.github.io/mediapipe/solutions/pose.html) */
class WEBCAMMOTIONCAPTURE_API UPoseDetector : public UObject
{
	GENERATED_BODY()
	
protected:
	/** OnnxModel of BlazePose Detection */
	OnnxModel* onnxModel;
	/** Buffer for the output detection */
	TArray<float> outputPoints;
	/** Buffer for the output confidence */
	TArray<float> outputConfidence;

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
		* Estimates the positions of human head and pelvis in the image
		* @param inputRawImage Input image as an array of bytes whose length is H(192) x W(192) x 3(BGR).
		* @param normalizedPelvisPosition Position of human pelvis the image, normalized to 0 to 1.
		* @param normalizedHeadPosition Position of human head the image, normalized to 0 to 1.
		* @return Whether the detection successed
		*/
		bool detect(UPARAM(ref) TArray<uint8>& inputRawImage, FVector2D& normalizedPelvisPosition, FVector2D& normalizedHeadPosition);
};
