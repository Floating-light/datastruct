```c++
DECLARE_DELEGATE_RetVal_OneParam(FString, FGetFieldPlayerStringValue, const FString& /*PlayerID*/);

class SStatisticsVisual;
FString SStatisticsVisual::GetTargetStringValue(const FString& RowStrName, const FString& PlayerID)  const
{

}

const FString&  RowName = TEXT("");
FGetFieldPlayerStringValue::CreateRaw(this, &SStatisticsVisual::GetTargetStringValue, RowName);
// error : no match instance of function.
// the second paramater type of GetTargetStringValue must be non reference
```