// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "EyeEstimator.generated.h"

UCLASS(Blueprintable)
/** Manages a OnnxModel of Iris Landmark from [MediaPipe Iris](https://google.github.io/mediapipe/solutions/iris.html) */
class WEBCAMFACIALCAPTURE_API UEyeEstimator : public UObject
{
	GENERATED_BODY()

protected:
	/** OnnxModel of Iris Landmark */
	OnnxModel* onnxModel;
	/** Buffer for the output eye lid positins */
	TArray<FVector> outputEyePoints;
	/** Buffer for the output iris positins */
	TArray<FVector> outputIrisPoints;

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
		* Crop the image used for BlazeFace Landmark
		* @param faceRawImage Input image as an array of bytes whose length is H(192) x W(192) x 3(BGR).
		* @param center The center of the image after cropping
		* @param eyeRawImage Output image as an array of bytes whose length is H(64) x W(64) x 3(BGR).
		* @param size Cropping size in the input image space
		*/
		void crop(const TArray<uint8>& faceRawImage, const FVector2D& center, TArray<uint8>& eyeRawImage, const int size = 30);

	UFUNCTION(BlueprintCallable, Category = "Facial Capture") 
		/**
		* Estimates the eye landmarks from image
		* @param inputRawImage Input image as an array of bytes whose length is H(64) x W(64) x 3(BGR).
		* @param out_eyePoints Positions of landmarks of eye lid in the image space.
		* @param out_irisPoints Positions of landmarks of iris in the image space.
		*/
		void capture(UPARAM(ref) TArray<uint8>& eyeRawImage, TArray<FVector>& out_eyePoints, TArray<FVector>& out_irisPoints);
};
