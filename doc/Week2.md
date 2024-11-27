1. 实现了被攻击后就回缩放然后能够得分的cube。这里采用了继承ActorComponent
的思路，然后再在其内部储存一个指向UPrimitiveComponent的指针.
```
UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scale")  
TObjectPtr<UPrimitiveComponent> PrimitiveComponent;

// 在beginplay的时候调用，获取对应Actor的UPrimitiveComponent 组件，因此改组件需要所挂的Actor 身上有UPrimitiveComponent.
void UTaoYanScalableComponent::SetScaleComponent(UPrimitiveComponent* InPrimitiveComponent)  
{  
    PrimitiveComponent = InPrimitiveComponent;  
    if (PrimitiveComponent)  
    {       PrimitiveComponent->SetSimulatePhysics(true);  
       PrimitiveComponent->OnComponentHit.AddDynamic(this, &UTaoYanScalableComponent::OnHit);  
    }}
```
2. 实现了限时得分的功能，Score 这里直接使用了UE自带的Score，没有自己添加.
```
void ATaoYanGameMode::BeginPlay()  
{  
    Super::BeginPlay();  
    //游戏开始时开始计时
    StartScoreTime();  
}

void ATaoYanGameMode::StartScoreTime()  
{  
	//开始时重置剩余时间
    GetGameState<ATaoYanGameState>()->SetRemainTime(TimeDuration); 
    //设定每秒调用一次方法以此来计算时间，这里就没有在tick中实现对应功能了。
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATaoYanGameMode::UpdateTime, 1.0f, true);  
}

void ATaoYanGameMode::UpdateTime()  
{  
    GetGameState<ATaoYanGameState>()->SetRemainTime(  
       GetGameState<ATaoYanGameState>()->GetRemainTime() -  
       GetWorld()->GetTimerManager().GetTimerRate(TimerHandle));  
    if (GetGameState<ATaoYanGameState>()->GetRemainTime() <= 0.0f)  
    {       GetWorld()->GetTimerManager().ClearTimer(TimerHandle);  
       // Score Time Over  
       UE_LOG(LogTemp, Warning, TEXT("Game Over"));  
       for (TObjectPtr PlayerState : GetWorld()->GetGameState()->PlayerArray)  
       {          if (const auto TaoYanPlayerState = Cast<ATaoYanPlayerState>(PlayerState))  
          {             UE_LOG(LogTemp, Warning, TEXT("Player %s Score: %f"), *TaoYanPlayerState->GetPawn()->GetName(),  
                    TaoYanPlayerState->GetScore());  
          }       }    }}

```
3. 最后实现了随机标记cube为special的功能。
```
void ATaoYanGameMode::RandomMarkSpecialScoreCube(int Special)  
{  
    TArray<AActor*> FoundActors;  
    UGameplayStatics::GetAllActorsOfClass(this, ScoreCubeClass, FoundActors);  
    if (FoundActors.Num() > 0)  
    {       const auto Indices = GetRandomUniqueNumbers(FoundActors.Num(), Special);  
       for (int i = 0; i < Indices.Num() && i < FoundActors.Num(); ++i)  
       {          const int Index = Indices[i];  
          if (const auto ScalableComponent = FoundActors[Index]->GetComponentByClass<UTaoYanScalableComponent>())  
          {             ScalableComponent->SetSpecial(true);  
          }       }    }}
          
```
也是在gamemode里面实现功能，然后再level 蓝图 beginplay 中进行调用。