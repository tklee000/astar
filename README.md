# develop_astar

A* 알고리즘

구현하기는 정말 쉽다~ 하지만 어떻게 이런 기발한 아이디어를 이해하는가는 별개의 문제 ^^;

```
F = G + H
G = 시작노드부터 현재노드까지 총 Cost 한칸 진행할때마다 비용이 1이며 이러한 값을 쭉 누적시킨다.
H = 현재노드부터 목표노드까지 총 Cost 일단 앞으로의 진행길을 모르기 때문에 그냥 목표점까지 벽뚫고 직선을 추정한 길이~

F = 목표점까지 가는 총Cost이다...쉽게 말해서 지나온 비용 + 지나길 비용 을 합산한 값이며,
최단거리로 향할 경우보다 당연히 둘러서가서 가는 경우가 비용이 커진다.
```

현재의 좌표에서 진행 가능한 다음 셀의 비용을 계산한 후 오픈리스트에 추가한다.
동서남북 4방향이 뚤려있다면 오픈리스트에 처음 4개가 추가시킨다.
오픈리스트는 우선순위큐(priority_queue)를 이용하는데 배열은 항상 적은 값 기준으로 정렬되기 때문에
추가된 노드중에서 비용을 가장적은 노드를 우선적으로 꺼내서 다음 단계를 처리할수 있게 된다.

오픈리스트 큐에서 노드를 하나 꺼낸후 노드의 좌표를 현재점으로 한 후 새로 진행한 노드에서 진행 가능한 방향의 비용을 계산한 후 탐색 노드를 추가한다.
이전에 현재 처리되는 좌표는 방문처리를 미리해두어서 지나온 곳은 또다시 계산하지 않는다. 
지나온 곳을 제외시켜서 나머지 3곳을 노드가 살펴본다.

참고로) 이미 지나온곳은 CloseList에 추가시켜서 제외 시킬것인지 여부를 확인할 수 있다. 
속도향상을 위해서 CloseList는 Push,Pop 과 같은 Vector 배열값이 아닌 그냥 맵과 동일한 크기의 배열을 사용해서 Check 플랙을 넣어서 구현하는것이 좋다.
CloseList에서 배열을 뒤지는 것보다는 빠른 연산이 가능하기 때문이다.

살펴보는 셀의 f 값이 무한대인 경우는 최초로 살펴보는 셀이기 때문에 무조건 오픈리스트에 추가한다.
바로 직전 노드에서도 4방향을 살폈기 때문에 이전 단계에서 계산된 셀을 참조하는 경우도 있는데 
만약에 지금의 f 가 더 작다면 셀의 내용을 갱신한고 오픈리스트에 추가한다.

탐색하면서 셀의 F,G,H, 부모X,부모Y 값은 계속 비용이 적은 상태로 셀의 내용을 갱신해놓는다.
셀은 어느 부터 부모 셀로부터 왔는가와 시작점부터 누적된 g 값이 누적되어 있다.
여기서 과거 셀의 f 보다 지금의 f 가 큰경우는 오픈리스트에 추가하지 않는 이유는 
전에 살펴본 지점이며 그쪽으로 가는경우는 오히려 돌아가야 하는 상황이라 오픈리스트에 추가하지 않는다.

그리고 정렬큐에서 최상위 노드다시 꺼내서 위의 과정을 반복한다.
정렬큐에서 최상위 노드를 꺼내는것이 자동으로 비용이 가장 적은 녀석을 우선적으로 꺼내진다. 그래서 가장 빠른 노드로 한단계식 진행할 수 있도록 해준다.
진행방향이 막히는 경우는 우선순위에서 밀려서 큐에서만 맴도는 처리되지 않은 노드에서 꺼내서 새로운 탐색을 할 수 있는 상태로 된다.