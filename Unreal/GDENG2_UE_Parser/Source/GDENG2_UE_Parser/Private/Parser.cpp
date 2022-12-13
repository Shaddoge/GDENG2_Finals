// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser.h"


UParser::UParser()
{
}

UParser::~UParser()
{
}

TArray<AMyPrimitive*> UParser::Load(FString fileName)
{
	TArray<AMyPrimitive*> myPrimitives;
	GLog->Log(FPaths::ProjectContentDir());
	const FString filePath = FPaths::ProjectContentDir() + "/LevelFiles/" + fileName;
	FString jsonString;

	FFileHelper::LoadFileToString(jsonString, *filePath);

	GLog->Log(jsonString);

	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);

	if (FJsonSerializer::Deserialize(jsonReader, jsonObject) && jsonObject.IsValid())
	{
		for (auto obj = jsonObject->Values.CreateConstIterator(); obj; ++obj)
		{
			TSharedPtr<FJsonValue> currValue = (*obj).Value;
			TSharedPtr<FJsonObject> currObj = currValue->AsObject();

			
			AMyPrimitive* primitive = GetWorld()->SpawnActor<AMyPrimitive>(AMyPrimitive::StaticClass());

			TArray<TSharedPtr<FJsonValue>> posArray = currObj->GetArrayField("position");
			FVector pos = FVector(posArray[0]->AsObject()->GetNumberField("x"), posArray[0]->AsObject()->GetNumberField("z"), posArray[0]->AsObject()->GetNumberField("y"));
			primitive->SetActorLocation(pos * 100.0f);

			TArray<TSharedPtr<FJsonValue>> rotArray = currObj->GetArrayField("rotation");
			
			FRotator rot = FRotator(FMath::RadiansToDegrees(rotArray[0]->AsObject()->GetNumberField("x")), FMath::RadiansToDegrees(rotArray[0]->AsObject()->GetNumberField("y")), FMath::RadiansToDegrees(rotArray[0]->AsObject()->GetNumberField("z")));
			primitive->SetActorRotation(rot);

			TArray<TSharedPtr<FJsonValue>> scaleArray = currObj->GetArrayField("scale");
			FVector scale = FVector(scaleArray[0]->AsObject()->GetNumberField("x"), scaleArray[0]->AsObject()->GetNumberField("z"), scaleArray[0]->AsObject()->GetNumberField("y"));
			primitive->SetActorRelativeScale3D(scale);
			//primitive->SetActorScale3D(scale);
			//AMyPrimitive* primitive = GetWorld()->SpawnActorDeferred<AMyPrimitive>(AMyPrimitive::StaticClass(), FTransform(rot, pos, scale));
			
			primitive->SetActorLabel(currObj->GetStringField("name"));
			
			bool hasPhysics = currObj->GetBoolField("hasPhysics");
			primitive->GetStaticMeshComponent()->SetSimulatePhysics(hasPhysics);
			if (hasPhysics)
				primitive->SetMobility(EComponentMobility::Movable);

			primitive->primitiveType = (EnumPrimitiveType)currObj->GetIntegerField("type");
			myPrimitives.Add(primitive);
		}
	}
	return myPrimitives;
}

void UParser::Save(FString fileName, TArray<AMyPrimitive*> myPrimitives)
{
	const FString filePath = FPaths::ProjectContentDir() + "/LevelFiles/" + fileName + ".level";
	
	TSharedPtr<FJsonObject> jsonObject;
	FString jsonString = "{";
	TArray<FString> data;
	
	for (int i = 0; i < myPrimitives.Num(); i++)
	{
		AMyPrimitive* myPrimitive = myPrimitives[i];
		jsonString += CreateStringJsonObject(FString::FromInt(i));
		jsonString += "{";
		jsonString += CreateStringJsonObject("hasPhysics");
		jsonString += myPrimitive->GetStaticMeshComponent()->IsSimulatingPhysics() ? "true," : "false,";
		jsonString += CreateStringJsonObject("name");
		jsonString += "\"" + myPrimitive->GetActorLabel() + "\",";
		jsonString += CreateStringJsonObject("position");
		jsonString += CreateArrayFromVector(myPrimitive->GetActorLocation() / 100.0f) + ",";
		jsonString += CreateStringJsonObject("rotation");

		FVector rotDeg = FVector(myPrimitive->GetActorRotation().Pitch, myPrimitive->GetActorRotation().Roll, myPrimitive->GetActorRotation().Yaw);
		jsonString += CreateArrayFromVector(FVector(FMath::DegreesToRadians(rotDeg.X), FMath::DegreesToRadians(rotDeg.Y), FMath::DegreesToRadians(rotDeg.Z))) + ",";
		jsonString += CreateStringJsonObject("scale");
		jsonString += CreateArrayFromVector(myPrimitive->GetActorScale()) + ",";
		jsonString += CreateStringJsonObject("type");
		jsonString += FString::FromInt((int)myPrimitive->primitiveType);
		jsonString += "},";
	}
	jsonString += "}";
	FFileHelper::SaveStringToFile(jsonString, *filePath);
}

FString UParser::CreateStringJsonObject(FString key)
{
	return "\"" + key + "\" : ";
}

FString UParser::CreateArrayFromVector(FVector vector)
{
	FString arrayString = "[";

	arrayString += "{";
	arrayString += CreateStringJsonObject("x");
	arrayString += FString::SanitizeFloat(vector.X) + ",";
	arrayString += CreateStringJsonObject("y");
	arrayString += FString::SanitizeFloat(vector.Z) + ",";
	arrayString += CreateStringJsonObject("z");
	arrayString += FString::SanitizeFloat(vector.Y);
	arrayString += "}]";

	return arrayString;
}
