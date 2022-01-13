
#define DELAY(UserObject, Function, Delay)											\
	FTimerHandle TimerHandle;														\
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, Delay, false);	\

#define DELAY_WITH_PARAMS(UserObject, Function, Delay, ...)												\
	const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(UserObject, Function, __VA_ARGS__);	\
	FTimerHandle TimerHandle;																			\
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, Delay, false);						\