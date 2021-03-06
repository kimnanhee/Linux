# 리눅스 프로젝트 개요

1. Text File에 기록된 S/W 블록 정보로부터 여러 S/W 블록들을 초기화시킨다.

2. S/W 블록의 이상동작(블럭 다운) 발생 시 재초기화를 수행한다.

3. 각 S/W 블록의 최종 초기화 시간 및 재초기화 회수를 출력한다.



-------

#### 1. S/W 블록 초기화

초기화해야하는 S/W 블록 정보는 아래와 같은 형식의 Text File로 기술된다. Text File에는 하나 이상의 S/W 블럭이 각각 서로 다른 수의 파라미터를 입력으로 받는다. S/W 블록 수는 최대 10개 이며, 파리미터의 수는 최대 3개이다.

| SwBlock1; | App1_Para1 ; | App1_Para2 ; |            |
| --------- | ------------ | ------------ | ---------- |
| SwBlock1; | App2_Para1 ; | App2_Para2 ; | App2_Para3 |
| ...       |              |              |            |
| SwBlock1; | AppN_Para1   |              |            |



#### 2. S/W 블록 재초기화

Application이 S/W 블록을 초기화한 후 특정 S/W 블록이 이상동작(블럭 다운)하는 경우, S/W 블록은 Application으로 블럭 다운에 따른 Signal을 전송한다. Application은 S/W 블록에서 전송되는 Signal을 획득하여 해당 S/W 블록을 재기동하고, 초기화 및 재기동한 정보를 기록하여야한다.

​    

\* 초기화 및 재기동 정보는 “./log/restart.txt"에 기록한다.

\* 초기화 및 재기동 정보에는 아래의 정보들이 포함되어야한다.

  \- 초기화 및 재기동한 S/W 명

  \- 초기화 및 재기동한 시점

  \- 초기화 및 재기동한 사유

  \- 초기화 및 재기동 회수



#### 3. S/W 블록 기동 정보 조회

S/W 블록이 초기화 또는 재초기화 되는 경우 Application은 전체 S/W 블록의 재초기화 시간 및 재초기화 회수를 아래와 같은 양식에 따라 출력한다. 아래 양식은 S/W 블록을 재초기화 시킬 때마다 출력한다.

| S/W Block Name | Restart Count | Start Time           | Reason                                          |
| -------------- | ------------- | -------------------- | ----------------------------------------------- |
| SwBlock1       | 1             | 2020.06.30. 15:20:33 | Init.                                           |
| SwBlock2       | 5             | 2020.06.30. 15:20:33 | Signal(Signal No & String)ex. Signal(9, Killed) |
| SwBlock3       | 2             | 2020.06.30. 15:20:33 | Exit(ExitVal)(ex. Exit(5))                      |
| ...            |               |                      |                                                 |
| SwBlockN       | 1             | 2020.06.30. 15:20:40 | Init.                                           |