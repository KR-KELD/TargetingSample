# TargetingSample

이 프로젝트는 언리얼 엔진을 사용하여 개발된 AI 타겟팅 샘플입니다.


## 프로젝트 소개

Behavior Tree의 Targeting Service Node를 통해 다양한 타겟팅 조건을 설정해 AI가 원하는 타겟을 지정 할 수 있게하였습니다.

타겟팅 선별 단계는 세 단계로 나누어집니다.
1. PawnSensingComponent에서 Pawn 타입의 오브젝트를 감지합니다.
2. CheckCondition(검증 조건)을 검사해 조건에 맞는 오브젝트만 선별합니다.
3. SelectCondition(선택 조건)과 SelectType(선택 방법)을 검사해 적절한 타겟을 선택합니다.

타겟팅 선별은 여러가지 조건을 추가해서 최선책, 차선책 등을 지정 할 수 있습니다.

## 설치 및 실행 방법
1. 프로젝트를 클론합니다.
2. TargetingSample.uproject파일을 우클릭하여 Generate Visual Studio project files를 누릅니다.
3. TargetingSample.sln 파일을 실행하여 Development Editor옵션을 선택 후 프로젝트를 컴파일 해줍니다. 
4. UE5 프로젝트를 실행합니다.

## 타겟팅 편집 방법
1. AI 폴더의 Behavior Tree 오브젝트 BT_Targeting을 들어가 Targeting Service 노드를 편집해 원하는 타겟팅 형태를 디자인합니다.

