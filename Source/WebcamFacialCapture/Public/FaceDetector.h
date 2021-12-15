// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "FaceDetector.generated.h"

USTRUCT(BlueprintType)
/** Container for detection results */
struct FFaceDetection
{
	GENERATED_BODY()

public:
	/** Calculate the average face position and orientation from output of the ML model */
	void calculateCenterOrientationAndSize()
	{
		center = (earR + earL + mouth + nose) * 0.25f;

		orientation = mouth - (eyeL + eyeR) * 0.5f;
		orientation.Normalize();

		TArray<FVector2D*> keypoints = { &eyeR, &eyeL, &nose, &mouth, &earR, &earL };
		float maxDistSquared = 0;
		for (FVector2D* keypoint : keypoints)
		{
			const float distSquared = FVector2D::DistSquared(center, *keypoint);
			if (maxDistSquared < distSquared)
			{
				maxDistSquared = distSquared;
			}
		}
		size = FMath::Sqrt(maxDistSquared);
	}

	/** Buffer for the output of ML model */
	FVector2D dummy0;
	/** Buffer for the output of ML model */
	FVector2D dummy1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the right eye */
		FVector2D eyeR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the left eye */
		FVector2D eyeL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the nose */
		FVector2D nose;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the mouth */
		FVector2D mouth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the right ear */
		FVector2D earR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the left ear */
		FVector2D earL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** Position of the center of the face calculated from output of the ML model */
		FVector2D center;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** The normalized vector from the center of the eyes to the mouth */
		FVector2D orientation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Facial Capture") 
		/** The max length from the 'center' to each keypoints */
		float size;
};

UCLASS(Blueprintable)
/** Manages a OnnxModel of BlazeFace Detection from [MediaPipe Face Detection](https://google.github.io/mediapipe/solutions/face_detection.html) */
class WEBCAMFACIALCAPTURE_API UFaceDetector : public UObject
{
	GENERATED_BODY()
	
protected:
	/** OnnxModel of BlazeFace Detection */
	OnnxModel* onnxModel;
	/** Buffer for the output detection */
	FFaceDetection detection;
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
		* Estimates the facial landmarks and average face position in the image
		* @param inputRawImage Input image as an array of bytes whose length is H(256) x W(256) x 3(BGR).
		* @param normalizedDetection Facial landmarks estimated from the image. The positions are normalized to 0 to 1 in the image.
		* @return Whether the detection successed
		*/
		bool detect(UPARAM(ref) TArray<uint8>& inputRawImage, FFaceDetection& normalizedDetection);
};
