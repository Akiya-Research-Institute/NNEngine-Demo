// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "OnnxModel.h"
#include "FaceEstimatorAttention.generated.h"

USTRUCT(BlueprintType)
/** Positions of facial landmarks */
struct FFaceMeshAttention
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture")
		float confidence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture") 
		/** Facial landmarks output from the ML model. See "Facial_landmarks_visualization.png" */
		TArray<FVector> face;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture") 
		/** 2D left eye landmarks */
		TArray<FVector2D> eyeL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture")
		/** 2D right eye landmarks */
		TArray<FVector2D> eyeR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture")
		/** 2D left iris landmarks */
		TArray<FVector2D> irisL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture")
		/** 2D right iris landmarks */
		TArray<FVector2D> irisR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Capture")
		/** 2D lips landmarks */
		TArray<FVector2D> lips;
};

UCLASS(Blueprintable)
/** Manages a OnnxModel of BlazeFace Mesh from [MediaPipe Face Mesh](https://google.github.io/mediapipe/solutions/face_mesh.html) */
class WEBCAMFACIALCAPTURE_API UFaceEstimatorAttention : public UObject
{
	GENERATED_BODY()

protected:
	/** OnnxModel of BlazeFace Mesh */
	OnnxModel* onnxModel;
	/** Buffer for the output mesh */
	FFaceMeshAttention faceMeshAttention;

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
		bool capture(UPARAM(ref) TArray<uint8>& inputRawImage, FFaceMeshAttention& outputMesh);

	UFUNCTION(BlueprintCallable, Category = "Facial Capture")
		/**
		* Calulate the average position, orientation, and size of the face in the image
		* @param center The position of pelvis
		* @param orientation The normalized vector perpendicular to the vector from the left eye to the right eye
		* @param size The max difference of X or Y coordinate between facial landmarks
		*/
		void calcNextRoiDiff(FVector2D& center, FVector2D& orientation, float& size);
};
