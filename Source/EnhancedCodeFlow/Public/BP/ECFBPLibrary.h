// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

/**
 * Library of static functions used to launch Code Flow functions via Blueprints.
 */

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BP/ECFHandleBP.h"
#include "ECFActionSettings.h"
#include "ECFTypes.h"
#include "ECFInstanceIdBP.h"
#include "ECFBPLibrary.generated.h"

UENUM(BlueprintType)
enum class ETimeLockOutputType : uint8
{
	Exec UMETA(DisplayName = "Exec"),
	Locked UMETA(DisplayName = "Locked")
};

UCLASS()
class ENHANCEDCODEFLOW_API UECFBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*^^^ ECF Flow Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Sets if the ECF system is paused or not.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Set Pause Subsystem"), Category = "ECF")
	static void ECFSetPause(const UObject* WorldContextObject, bool bPaused);

	/**
	 * Checks if the ECF system is paused or not.
	 */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Set Pause Subsystem"), Category = "ECF")
	static void ECFGetPause(const UObject* WorldContextObject, UPARAM(DisplayName = "IsPaused") bool& bIsPaused);

	/**
	 * Checks if the action pointed by given handle is running.
	 */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Is Action Running"), Category = "ECF")
	static void ECFIsActionRunning(UPARAM(DisplayName = "IsRunning") bool& bIsRunning, const UObject* WorldContextObject, const FECFHandleBP& Handle);

	/**
	 * Pause running action.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Pause Action"), Category = "ECF")
	static void ECFPauseAction(const UObject* WorldContextObject, const FECFHandleBP& Handle);

	/**
	 * Resume running action.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Resume Action"), Category = "ECF")
	static void ECFResumeAction(const UObject* WorldContextObject, const FECFHandleBP& Handle);

	/**
	 * Checks if the action pointed by given handle is paused.
	 * Returns false if there is no action running.
	 */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", DisplayName = "ECF - Is Action Paused"), Category = "ECF")
	static void ECFIsActionPaused(UPARAM(DisplayName = "IsRunning") bool& bIsRunning, UPARAM(DisplayName = "IsPaused") bool& bIsPaused, const UObject* WorldContextObject, const FECFHandleBP& Handle);
	
	/*^^^ Stopping ECF Functions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Stops the running action pointed by given handle. Invalidates given handle.
	 * bComplete param indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete", DisplayName = "ECF - Stop Action"), Category = "ECF")
	static void ECFStopAction(const UObject* WorldContextObject, UPARAM(ref) FECFHandleBP& Handle, bool bComplete = false);

	/**
	 * Stops the running action with the given InstanceId.
	 * bComplete param indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete", DisplayName = "ECF - Stop Instanced Actions"), Category = "ECF")
	static void ECFStopInstancedActions(const UObject* WorldContextObject, FECFInstanceIdBP InstanceId, bool bComplete = false);

	/**
	 * Stops all running actions.
	 * If owner is defined it will remove all actions from the given owner.
	 * Otherwise it will stop all the actions from everywhere.
	 * bComplete param indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Stop All Actions"), Category = "ECF")
	static void ECFStopAllActions(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/*^^^ Handle and Instance Id ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	
	/**
	 * Checks if the given Handle is valid.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ECF - Is Handle Valid"), Category = "ECF")
	static void IsECFHandleValid(UPARAM(DisplayName = "IsValid") bool& bOutIsValid, const FECFHandleBP& Handle);

	/**
	 * Returns a new Instance Id.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ECF - Get New Instance Id"), Category = "ECF")
	static void ECFGetNewInstanceId(UPARAM(DisplayName = "InstanceId") FECFInstanceIdBP& OutInstanceId);

	/**
	 * Check if given InstanceId is Valid and Validate it (creates new one) if not.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ECF - Validate Instance Id"), Category = "ECF")
	static void ECFValidateInstanceId(UPARAM(Ref, DisplayName="InstanceId") FECFInstanceIdBP& InInstanceId, UPARAM(DisplayName = "InstanceId") FECFInstanceIdBP& OutInstanceId);

	/**
	 * Checks if the given InstanceId is valid.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ECF - Is Instance Id Valid"), Category = "ECF")
	static void IsECFInstanceIdValid(UPARAM(DisplayName = "IsValid") bool& bIsValid, const FECFInstanceIdBP& InstanceId);

	/*^^^ Time Lock ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Allow to run the code only once in a given time. (Locks the ability to run the code for a specific amount of time in seconds).
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "Owner", DefaultToSelf = "Owner", ExpandEnumAsExecs = "OutExecs", AdvancedDisplay = "Settings", DisplayName = "ECF - Time Lock"), Category = "ECF")
	static void ECFTimeLock(ETimeLockOutputType& OutExecs, UPARAM(DisplayName = "Handle") FECFHandleBP& OutHandle, UObject* Owner, float LockTime, UPARAM(Ref) FECFInstanceIdBP& InstanceId, FECFActionSettings Settings);

	/**
	 * Removes all time locks.
	 * @param InOwner [optional] - if defined it will remove time locks only from the given owner.
	 *                             Otherwise it will remove all time locks from everywhere.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "InOwner", DisplayName = "ECF - Remove All Time Locks"), Category = "ECF")
	static void ECFRemoveAllTimeLocks(const UObject* WorldContextObject, UObject* InOwner = nullptr);


	/*^^^ Removing Async Actions ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Removes all running delays.
	 * If owner is defined it will remove all delays from the given owner.
	 * Otherwise it will stop all the delays from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All Delays"), Category = "ECF")
	static void ECFRemoveAllDelays(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/**
	 * Removes all running "wait and execute"s.
	 * If owner is defined it will remove all "wait and execute"s from the given owner.
	 * Otherwise it will stop all the "wait and execute"s from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All Wait And Executes"), Category = "ECF")
	static void ECFRemoveAllWaitAndExecutes(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/**
	 * Removes all running "while true execute"s.
	 * If owner is defined it will remove all "while true execute"s from the given owner.
	 * Otherwise it will stop all the "while true execute"s from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All While True Executes"), Category = "ECF")
	static void RemoveAllWhileTrueExecutes(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/**
	 * Removes all running tickers.
	 * If owner is defined it will remove all tickers from the given owner.
	 * Otherwise it will stop all the tickers from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All Tickers"), Category = "ECF")
	static void ECFRemoveAllTickers(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/**
	 * Removes all running timelines.
	 * If owner is defined it will remove all timelines from the given owner.
	 * Otherwise it will stop all the timelines from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All Timelines"), Category = "ECF")
	static void ECFRemoveAllTimelines(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/**
	 * Removes all running custom timelines.
	 * If owner is defined it will remove all custom timelines from the given owner.
	 * Otherwise it will stop all the custom timelines from everywhere.
	 * bComplete indicates if the action should be completed when stopped (run callback), or simply stopped.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "bComplete, InOwner", DisplayName = "ECF - Remove All Custom Timelines"), Category = "ECF")
	static void ECFRemoveAllCustomTimelines(const UObject* WorldContextObject, bool bComplete = false, UObject* InOwner = nullptr);

	/*^^^ Casting ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

	/**
	 * Convert Handle to String.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (FECFHandleBP)", CompactNodeTitle = "->", BlueprintAutocast), Category = "ECF")
	static FString Conv_ECFHandleToString(const FECFHandleBP& Handle);

	/**
	 * InstanceId Handle to String.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (FECFInstanceIdBP)", CompactNodeTitle = "->", BlueprintAutocast), Category = "ECF")
	static FString Conv_ECFInstanceIdToString(const FECFInstanceIdBP& InstanceId);

	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
};
