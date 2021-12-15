// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "TextureProcessComponent.h"
#include "PoseEstimator.generated.h"

UCLASS(Blueprintable)
/** Manages a OnnxModel of Movenet Lightning from [TensorFlow HUB movenet/singlepose/lightning](https://tfhub.dev/google/movenet/singlepose/lightning/4) */
class WEBCAMMOTIONCAPTURE_API UPoseEstimator : public UObject
{
	GENERATED_BODY()
	
protected:
	/** OnnxModel of Movenet Lightning */
	OnnxModel* onnxModel;
	/** Buffer for the output */
	TArray<float> outputValues;

	/** Check if the confidence is enough */
	bool checkConfidence();
	/** Convert the output of ML model to an arrray of Vector2D */
	void parseToVector2d(TArray<FVector2D>& jointPositions);

public:
	UFUNCTION(BlueprintCallable, Category = "Motion Capture") 
		/**
		* Open ONNX model and initialize it.
		* @param onnxFilePath Path of the .onnx file
		* @param onnxProvider Execution provider used for this model
		* @param gpuDeviceId Which GPU to be used
		*/
		void init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId = 0);

	UFUNCTION(BlueprintCallable, Category = "Motion Capture") 
		/**
		* Estimates the positions of human joints in the image
		* @param inputRawImage Input image as an array of bytes whose length is H(128) x W(128) x 3(BGR).
		* @param normalizedJointPositions Position of human joints in the image, normalized to 0 to 1. The positions are in the order of [nose, left eye, right eye, left ear, right ear, left shoulder, right shoulder, left elbow, right elbow, left wrist, right wrist, left hip, right hip, left knee, right knee, left ankle, right ankle].
		* @return Whether the estimation successed
		*/
		bool capture(UPARAM(ref) TArray<uint8>& inputRawImage, TArray<FVector2D>& normalizedJointPositions);

	UFUNCTION(BlueprintCallable, Category = "Motion Capture")
		/**
		* Calulate the average position, orientation, and size of human in the image
		* @param jointPositions Position of human joints in the image
		* @param center The position of pelvis
		* @param orientation The normalized vector from the head to pelvis
		* @param size The max distance from pelvis to head, hands, and feet.
		*/
		void calcNextRoi(const TArray<FVector2D>& jointPositions, const FVector2D& uvScalingFactor, FVector2D& center, FVector2D& orientation, float& size);
};