#include<stdio.h>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include"Timer.h"
using namespace std;

#define MAX_VERTEX_NUM  200
typedef char TransKind ;
typedef char InfoType ;



typedef struct
{
	bool pass = false;
	char country[64] = {0};
	char city[64] = {0};
	double latitude;//γ��
	double longitude;//����
}City,VertexType;   //���У�����Ľṹ��

//�߽��
typedef struct ArcNode
{
	int adjvex;                     //�ڽӵĶ���
	struct ArcNode* nextarc;       //��һ���ڽӱ�ָ��
	TransKind kind[6] = { 0 };
	double time;
	double money;
	InfoType info[1024] = { 0 };
}ArcNode;

//�ڽӱ��еĸ�����ʼ����
typedef struct VNode {  
	VertexType data;
	ArcNode* firstArc=nullptr;

}VNode,AdjList[MAX_VERTEX_NUM];

//ͼ�Ľṹ
typedef struct {
	AdjList vertices;   //�ڽӱ�
	int vexnum=0;         //����
	int arcnum=0;        //����
}Graph;

bool CreateCities(const char* pathname, vector<City>& city)
{
	//�ú��������������б���city.csv�ж�ȡ������Ϣ  
	FILE *fp = nullptr;
	fopen_s(&fp,pathname, "rt");
	if (!fp) 
	{   
		printf("Cannot open the city file: %s!",pathname);   
		return false;  
		exit(0);
	}  
	City tmpcity;//��Ϊ������ʱ�洢����  
	int i = 0, j = 0;  
	char* record = NULL;  
	char* next_token =nullptr;   //�洢�ָ��ʣ���
	char buf[1024];//ÿһ�е���Ϣ������  
	char* line = NULL;  
	/*int count = 0;*/
	while ((line = fgets(buf, 1024, fp)) != NULL) 
	{   
		
		record = strtok_s(buf, ",",&next_token);   //��","��Ϊ�ָ�����ȡ��Ϣ   
		while (record != NULL) 
		{    
			switch (j) 
			{    
			case 0://��һ������ǰ��Ϊcountry     
				strcpy_s(tmpcity.country, record);     
				j++; 
				break; 
			case 1://�ڶ�������ǰ��Ϊcity     
				strcpy_s(tmpcity.city, record);     
				j++; 
			    break;    
			case 2://����������ǰ��Ϊγ��     
				tmpcity.latitude = atof(record); 
				j++; 
				break;    
			case 3://���ĸ�����ǰΪ����     
				tmpcity.longitude = atof(record);
				j = 0; 
				break;    
			}    
			record = strtok_s(NULL, "," ,&next_token); //�ڶ��ε��ã���ǰһ���ָ�ĵط���ȡ, ���û�ҵ��ָ������null
		}   
		city.push_back(tmpcity);   //����i�����д���city��  
	      //��ѭ��������һ�еĳ��ж�ȡ
		/*count++;
		if (count == 99)break;*/
	} 

	fclose(fp);
	printf("��ȡcity�ɹ�\n");
	return true;
} 

int SearchCity(const char* des, vector<City>&city) 
{  //�ú�������ĳ����des�ڳ��б�city�е��±����  
	for (int pos = 0; pos < int(city.size()); pos++) 
	{   
		if (!strcmp(city[pos].city, des))
			return pos;// �ҵ��򷵻��±� 
	}  
	return -1;     //��δ�ҵ��򷵻�-1 
} 

//�˺���������ͼʱ���ظ�����ϢҲ��ȡ�˲��洢��
bool CreateGraph(const char* pathname,Graph& G, vector<City>city)
{  //�ú������ڽ���������ͼG��������Ϣ����city���ߵ���Ϣ����route.csv�ļ�  

	//��ʼ��
	G.vexnum = city.size();//������Ϊcity����  
	G.arcnum = 0;//������ʼΪ0  
	for (int i = 0; i < G.vexnum; i++) 
	{
		//��ʼ����������  
		G.vertices[i].data = city[i];
		G.vertices[i].firstArc = nullptr;
	}  

	FILE* fp = nullptr;
	fopen_s(&fp, pathname, "rt");
	if (!fp) 
	{   
		printf("Cannot open the route file: %s",pathname);
		return false;
	}  

	char* record = nullptr;
	char* next_token = nullptr;  //�洢�ָ��ʣ���
	char buf[1024];        //ÿһ�е���Ϣ������ 
	char* line = nullptr;

	while ((line = fgets(buf, 1024, fp)) != nullptr) 
	{   
		int j = 0;      //j����ѭ������
		int v1, v2;     //v1Ϊ��㣬v2 Ϊ�յ�   
		ArcNode* tmparc = new ArcNode;    //tmparc��ʱ�������Ϣ   
		tmparc->nextarc = nullptr;

		record = strtok_s(buf, ",",&next_token); 
		while (j <= 6) 
		{    
			if (j == 0) 
			{   //j == 0ʱ�ҵ�������     
				char temp[1024] = { 0 };      //temp���ڴ洢��������֮���ַ�����Ϣ
				strcpy_s(temp, record);
				v1 = SearchCity(temp, city);  //v1Ϊ����±�     
				if (v1 == -1)break;
				record = strtok_s(NULL, ",",&next_token);
				j++;
			}
		   else if (j == 1) 
			{   //j == 1ʱ�ҵ��յ����     
				char temp[1024] = { 0 };
				strcpy_s(temp, record);
				v2 = SearchCity(temp, city);     //v2Ϊ�յ��±�
				if (v2 == -1)break;
				tmparc->adjvex = v2;           //�ñߵ�adjvexΪv2

				record = strtok_s(NULL, ",", &next_token);
				j++;    
			}    
		   else if (j == 2) 
			{    //j == 2ʱ��ȡ��ͨ��ʽ     
				char temp[1024] = { 0 };
				strcpy_s(temp, record); 

				if (!strcmp(temp, "plane")) 
				{     
					strcpy_s(tmparc->kind, "plane");
				}     
				else if (!strcmp(temp, "bus")) 
				{      
					strcpy_s(tmparc->kind, "bus");  
				}     
				else if (!strcmp(temp, "train")) 
				{      
					strcpy_s(tmparc->kind, "train");
				}     

				record = strtok_s(NULL, ",", &next_token);
				j++;    
			}   
			else if (j == 3) 
			{   //j == 3ʱ��������ʱ��     
				char temp[1024] = { 0 };
				strcpy_s(temp, record);
				tmparc->time = atof(temp); 

				record = strtok_s(NULL, ",", &next_token);
				j++;
			}    
			else if (j == 4) 
			{   //j == 4ʱ�����������    
				char temp[1024] = { 0 }; 
				strcpy_s(temp, record); 
				tmparc->money = atof(temp);         //atoi���ַ���ת��Ϊ����  atofΪdouble

				record = strtok_s(NULL, "\n", &next_token); 
				//���ں�����ϢΪ�ߵĸ�����Ϣ�����Բ��Զ���Ϊ�ָ��������Ի��з�Ϊ�ָ�������ȥ\n  
				j++;
			}    
			 
			else if (j == 5) 
			{   //j == 5ʱ���븽����Ϣ
				char temp[1024] = { 0 };
				strcpy_s(temp, record);
				//��Ϊinfo����'����ʱ�������html�п��ӻ�ʱ����С���⣬������Ҫ����\ת��
				char p[1024] = { 0 };
				int i = 0; int k = 0;
				if(strstr(temp, "\'"))
				{
					while (temp[k] != '\0')
					{
						while (temp[k] != '\''&& temp[k] != '\0')
						{
							p[i] = temp[k];
							i++, k++;
						}
						if (temp[k] == '\'')
						{
							p[i++] = '\\';
							p[i++] = '\'';
							k++;
						}
						else if (temp[k] == '\0')
						{
							p[i] = '\0';
						}
					}
				}
				strcpy_s(tmparc->info, p);
				j++;    
			}   
			else if (j == 6) 
			{   //j == 6ʱ���ñ����ӵ�v1���ڽӱ���ȥ
				if (G.vertices[v1].firstArc == nullptr)
				{
					G.vertices[v1].firstArc = tmparc;//��ͷ�ڵ�Ϊ�գ������ӵ�ͷ�ڵ� 
					G.vertices[v1].firstArc->nextarc = nullptr;
				}
				else 
				{   //��ͷ�ڵ㲻�գ������ӵ�β�����ȥ
					ArcNode* p= G.vertices[v1].firstArc;
					while (p->nextarc!=nullptr) 
					{      
						p = p->nextarc; 
					} 
					p->nextarc = tmparc;
					p->nextarc->nextarc = nullptr;
					G.arcnum++;
				}    
				j++; 
			}  
		} 
		//һ����Ϣ����
    } //whileѭ����ȡ����
	fclose(fp);
	printf("������ͼ�ɹ�\n");
	return true; 
}

bool First = true;
void DFS(Graph &G, int v0, vector<City>& city)
{
	if (First)
	{
		printf("%s", city[v0].city);
		First = false;
	}
	else 
	{
		printf("->%s", city[v0].city);
	}
	city[v0].pass = true;
	int v;
	ArcNode* p = G.vertices[v0].firstArc;
	while (p != nullptr)
	{
		v = p->adjvex;
		if (city[v].pass == false)
		{
			DFS(G, v, city);
		}
		p = p->nextarc;
	}
}
void BFS(Graph& G, int v0, vector<City>& city)
{
	int w;
	ArcNode* p=nullptr;
	queue<int> qu;		//���廷�ζ���ָ��
	printf("%s",city[v0].city); 		//��������ʶ���ı��
	city[v0].pass = true;              	//���ѷ��ʱ��
	qu.push(v0);
	while (!qu.empty())       	//�Ӳ���ѭ��
	{
		w=qu.front();			//����һ������w
		qu.pop();
		p = G.vertices[w].firstArc; 	//ָ��w�ĵ�һ���ڽӵ�
		while (p != nullptr)		//����w�������ڽӵ�
		{
			int v = p->adjvex;
			if (city[v].pass == false) 	//����ǰ�ڽӵ�δ������
			{
				printf("->%s",city[v].city);  //���ʸ��ڽӵ�
				city[v].pass = true;	//���ѷ��ʱ��
				qu.push(v);	//�ö������
			}
			p = p->nextarc;              	//����һ���ڽӵ�
		}
	}
	printf("\n");
}

double GetWeight(Graph &G, int v0, int v, char kind) 
{  //�ú������ڷ���v0��v�ıߵ�Ȩ�أ�����GΪ���õ�ͼ  
   //v0Ϊ��㣬vΪ�յ㣬kindΪ'T'ʱ�򷵻�����ʱ�䣬 Ϊ'M'ʱ�򷵻��������  
   //����v0��v�ж���һ�ַ�ʽ���򷵻�Ȩֵ��С��ֵ  
   //�������ڸñߣ��򷵻�INFINITY  
	double min = INFINITY*1.0;//��Сֵ��ʼ��Ϊ�����  
	if (kind == 'T') 
	{//������ʱ��   
		ArcNode *p = G.vertices[v0].firstArc;   
		while (p!=nullptr)   //�������д�����V0���ڵı�Ѱ���յ�Ϊv��
		{    
			if (p->adjvex == v) 
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min     
				if (p->time < min)
					min = p->time;    
			}   
			p = p->nextarc;
		}   
		return min;  
	}  
	else if (kind == 'M') 
	{//���������   
		ArcNode *p = G.vertices[v0].firstArc;
		while( p != nullptr) 
		{    
			if (p->adjvex == v) 
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min    
				if (p->money < min)
					min = p->money;    
			}   
			p = p->nextarc;
		}      
		return min;  
	} 
	else
	{
		printf("kind ��������\n");
		return 0;
	}
} 

double GetWeight2(Graph& G, int v0, int v, char Kind,char*kind)
{  //�ú������ڷ���v0��v�ıߵ�Ȩ�أ�����GΪ���õ�ͼ  
   //v0Ϊ��㣬vΪ�յ㣬kindΪ'T'ʱ�򷵻�����ʱ�䣬 Ϊ'M'ʱ�򷵻��������  
   //����v0��v�ж���һ�ַ�ʽ���򷵻�Ȩֵ��С��ֵ  
   //�������ڸñߣ��򷵻�INFINITY  
	double min = INFINITY * 1.0;//��Сֵ��ʼ��Ϊ�����  
	if (Kind == 'T')
	{//������ʱ��   
		ArcNode* p = G.vertices[v0].firstArc;
		while (p!= nullptr)   //�������д�����V0���ڵı�Ѱ���յ�Ϊv��
		{
			if (p->adjvex == v&& strcmp(p->kind,kind)==0)
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min     
				if (p->time < min)
					min = p->time;
			}
			p = p->nextarc;
		}
		return min;
	}
	else if (Kind == 'M')
	{//���������   
		ArcNode* p = G.vertices[v0].firstArc;
		while (p!= nullptr)
		{
			if (p->adjvex == v&& strcmp(p->kind, kind) == 0)
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min    
				if (p->money < min)
					min = p->money;
			}
			p = p->nextarc;
		}
		return min;
	}
	else
	{
		printf("kind ��������\n");
		return 0;
	}
}

typedef double ShortPathTable[MAX_VERTEX_NUM];
int Path[MAX_VERTEX_NUM];      //Path[]�д�����ÿ�����������·������һ������±꣬����������һ���㣬��Ϊ��㣬����ֵΪ-1 
void ShortestPath(Graph &G, int v0, ShortPathTable &D,char kind) 
{ 
	//�ú���ʹ��Dijkstra�㷨���ͼG����v0Ϊ��㵽������������·������ֵ��������D��  
   //kindΪ����Ȩֵ�����ͣ�'T'Ϊʱ��,'M'Ϊ����  
	if (kind != 'T' && kind != 'M')
	{
		printf("kind ��������\n");
		return;
	}
	int v, w, i;  
	double min;  
	int final[MAX_VERTEX_NUM];  //final[v]=1 ��ʾv0��v�����·������� 

	for (v = 0; v < G.vexnum; v++) 
	{//��ʼ��final[]��D[]��Path[]   
		final[v] =0; 
		D[v] = GetWeight(G, v0, v, kind);
		if (D[v] != INFINITY)
		{
			Path[v] = v0;
		}
		else
		{
			Path[v] = -1;
		}
	}  
	D[v0] = 0;   //��ʼ��D[v0] = 0,�����Լ������·��
	final[v0] = 1;     //��ʼ��final[v0] = 1 
   //��ʼ��ѭ����ÿ�����v0��ĳ��v��������·���� ����final��Ϊ1  
	for (i = 1; i < G.vexnum; i++) 
	{//������ʣ�µ� G.vexnum - 1������   
		min = INFINITY*1.0;//��ǰ��֪��v0������������   
		for (w = 0; w < G.vexnum; w++) 
		{    
			if (!final[w] && D[w] < min) //Ѱ��һ��ʼ����ֱ�ӵ���ǰ��i+1����������·����һ��ʼ����ֱ�����ӵ�V0�����ֵ��
			{ 
				v = w;
				min = D[w];
			}
			//w������v0�������   
		}   
		//ѭ�������ҵ������v
		final[v] = 1;//��v0���������v��final[v]��Ϊ1
		for (w = 0; w < G.vexnum; w++) 
		{//���µ�ǰ���·��������    
			if (final[w]==0 && (min + GetWeight(G, v, w, kind) < D[w]))  //���ͨ��������ǰ��min�Ķ���V�ٵ�����w��ֵ��һ��ʼ��¼�ĵ�w�����·���̾���Ҫ����
			{     
				D[w] = min + GetWeight(G, v, w, kind);
				Path[w] = v;    //��¼����w·������һ������v��ֵ    
			}   
		}  
	} 
}

void ShortestPath2(Graph& G, int v0, ShortPathTable& D, char Kind,char*kind)
{
	//�ú���ʹ��Dijkstra�㷨���ͼG����v0Ϊ��㵽������������·������ֵ��������D��  
   //kindΪ����Ȩֵ�����ͣ�'T'Ϊʱ��,'M'Ϊ����  
	if (Kind != 'T' && Kind != 'M')
	{
		printf("Kind ��������\n");
		return;
	}
	if (strcmp(kind,"bus")!=0 && strcmp(kind, "plane")!=0 && strcmp(kind, "train")!=0)
	{
		printf("��ͨ��ʽ ��������\n");
		return;
	}
	int v, w, i;
	double min;
	int final[MAX_VERTEX_NUM];  //final[v]=1 ��ʾv0��v�����·������� 

	for (v = 0; v < G.vexnum; v++)
	{//��ʼ��final[]��D[]��Path[]   
		final[v] = 0;
		D[v] = GetWeight2(G, v0, v, Kind,kind);
		if (D[v] != INFINITY)
		{
			Path[v] = v0;
		}
		else
		{
			Path[v] = -1;
		}
	}
	D[v0] = 0;   //��ʼ��D[v0] = 0,�����Լ������·��
	final[v0] = 1;     //��ʼ��final[v0] = 1 
   //��ʼ��ѭ����ÿ�����v0��ĳ��v��������·���� ����final��Ϊ1  
	for (i = 1; i < G.vexnum; i++)
	{//������ʣ�µ� G.vexnum - 1������   
		min = INFINITY * 1.0;//��ǰ��֪��v0������������   
		for (w = 0; w < G.vexnum; w++)
		{
			if (!final[w] && D[w] < min) //Ѱ��һ��ʼ����ֱ�ӵ���ǰ��i+1����������·����һ��ʼ����ֱ�����ӵ�V0�����ֵ��
			{
				v = w;
				min = D[w];
			}
			//w������v0�������   
		}
		//ѭ�������ҵ������v
		final[v] = 1;//��v0���������v��final[v]��Ϊ1
		for (w = 0; w < G.vexnum; w++)
		{//���µ�ǰ���·��������    
			if (final[w] == 0 && (min + GetWeight2(G, v, w, Kind,kind) < D[w]))  //���ͨ��������ǰ��min�Ķ���V�ٵ�����w��ֵ��һ��ʼ��¼�ĵ�w�����·���̾���Ҫ����
			{
				D[w] = min + GetWeight2(G, v, w, Kind,kind);
				Path[w] = v;    //��¼����w·������һ������v��ֵ    
			}
		}
	}
}


void GetArc(Graph& graph, int w1, int w2, ArcNode& arc, char kind)   //Ѱ��w1��w2�ıߴ浽arc��
{
	double min = INFINITY * 1.0;//��Сֵ��ʼ��Ϊ�����  
	if (kind == 'T')
	{
		ArcNode* p = graph.vertices[w1].firstArc;
		while (p != nullptr)   //�������д�����w1���ڵı�Ѱ���յ�Ϊw2��
		{
			if (p->adjvex == w2)
			{//���յ�Ϊw2���ж�Ȩֵ�Ƿ�С��min     
				if (p->time < min)
				{
					min = p->time;
					arc = *p;
				}
			}
			p = p->nextarc;
		}
	}
	else if (kind == 'M')
	{  //���������   
		ArcNode* p = graph.vertices[w1].firstArc;
		while (p != nullptr)
		{
			if (p->adjvex == w2)
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min    
				if (p->money < min)
				{
					min = p->money;
					arc = *p;
				}
			}
			p = p->nextarc;
		}
	}
}
void GetArc2(Graph& graph, int w1, int w2, ArcNode& arc, char Kind,char*kind)   //Ѱ��w1��w2��kind��ͨ��ʽ�ıߴ浽arc��
{
	double min = INFINITY * 1.0;//��Сֵ��ʼ��Ϊ�����  
	if (Kind == 'T')
	{
		ArcNode* p = graph.vertices[w1].firstArc;
		while (p != nullptr)   //�������д�����w1���ڵı�Ѱ���յ�Ϊw2��
		{
			if (p->adjvex == w2&&strcmp(p->kind,kind)==0)
			{//���յ�Ϊw2���ж�Ȩֵ�Ƿ�С��min     
				if (p->time < min)
				{
					min = p->time;
					arc = *p;
				}
			}
			p = p->nextarc;
		}
	}
	else if (Kind == 'M')
	{  //���������   
		ArcNode* p = graph.vertices[w1].firstArc;
		while (p != nullptr)
		{
			if (p->adjvex == w2 && strcmp(p->kind, kind) == 0)
			{//���յ�Ϊv���ж�Ȩֵ�Ƿ�С��min    
				if (p->money < min)
				{
					min = p->money;
					arc = *p;
				}
			}
			p = p->nextarc;
		}
	}
}

bool createhtml(Graph&graph,char*start,char*des,int v,char kind)
{
	FILE* fp = nullptr;
	fopen_s(&fp, "tmp.html", "wt");
	if (!fp)
	{
		printf("Cannot open the file:tmp.html\n");
		return false;
	}
	fprintf(fp, "<!DOCTYPE html><html><head><style type='text/css'>body, html{width: 100%%;height: 100%%;margin:0;font-family:'΢���ź� ';}#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}</style><script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script><title>Shortest path from %s to %s</title></head><body><div id='allmap'></div></div></body></html><script type='text/javascript'>var map = new BMap.Map('allmap');var point = new BMap.Point(0, 0);map.centerAndZoom(point, 2);map.enableScrollWheelZoom(true);", start, des);

	//���ݸ�ʽ���  
	stack<int>q;//����Path[]��ÿ�����㴢���Ϊ��һ��������±꣬���½�һ��ջ��ջ��Ϊ�յ㣬ջ��Ϊ��� 
	int w = v;//�յ�
	while (Path[w] != -1)
	{
		q.push(w);
		w = Path[w];
	}
	q.push(w); //�����ѹ��ջ��

	int j = 0;
	while (q.size() != 1) //�����Ϊ-1
	{
		int w1 = q.top();//��j�αߵĿ�ʼ����   //j=0�����
		q.pop();
		int w2 = q.top(); //��������
		ArcNode arc;
		GetArc(graph, w1, w2, arc, kind);
		fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point(%.4lf, %.4lf));map.addOverlay(marker%d);\n", j, graph.vertices[w1].data.longitude, graph.vertices[w1].data.latitude, j);
		fprintf(fp, "var infoWindow%d = new BMap.InfoWindow(\"<p style = 'fontsize:14px;'>country: %s<br/>city : %s</p>\");marker%d.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow%d);}); var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));map.addOverlay(marker%d);\n", j, graph.vertices[w1].data.country, graph.vertices[w1].data.city, j, j, j + 1, graph.vertices[w2].data.longitude, graph.vertices[w2].data.latitude, j + 1);
		fprintf(fp, "var infoWindow%d = new BMap.InfoWindow(\"<p style = 'fontsize:14px;'>country: %s<br/>city : %s</p>\");marker%d.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow%d);}); var contentString%.2d = '%s, %s --> %s, %s (%s - %.2f hours - $%.2f - %s)';var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#25a45b', strokeWeight:8, strokeOpacity:0.8});map.addOverlay(path%d);path%d.addEventListener(\"click\", function(){alert(contentString%.2d);});", j + 1, graph.vertices[w2].data.country, graph.vertices[w2].data.city, j + 1, j + 1, j + 1, graph.vertices[w1].data.city, graph.vertices[w1].data.country, graph.vertices[w2].data.city, graph.vertices[w2].data.country, arc.kind, arc.time, arc.money, arc.info, j + 1, graph.vertices[w1].data.longitude, graph.vertices[w1].data.latitude, graph.vertices[w2].data.longitude, graph.vertices[w2].data.latitude, j + 1, j + 1, j + 1);
		j++;
	}
	fprintf(fp, "</script>");
	fclose(fp);
	return true;
}

void help()
{
	
	printf("**************\n");
	printf("/*�˵�*/\n");
	printf("1.�����·��\n");
	printf("2.�涨��ͨ��ʽ�����·��\n");
	printf("3.��DFS·��\n");
	printf("4.��BFS·��\n");
	printf("5.help\n");
	printf("6.exit\n"); 
	
	printf("**************\n");
}
void outputDist(int v, ShortPathTable& D, char kind)
{
	if (kind == 'T')
	{
		printf("The shortest time is: %.2lf\n ", D[v]);
	}
	else if (kind == 'M')
	{
		printf("The shortest money is: %.2lf\n", D[v]);
	}
}
void outputPATH(int v, Graph& graph)
{
	stack<int>q;//����Path[]��ÿ�����㴢���Ϊ��һ��������±꣬���½�һ��ջ��ջ��Ϊ�յ㣬ջ��Ϊ��� 
	int w = v;//�յ�
	while (Path[w] != -1)
	{
		q.push(w);
		w = Path[w];
	}
	q.push(w); //�����ѹ��ջ��
	int first = 1;
	while (q.size() != 0)
	{
		int tmpw = q.top();//��j�αߵĿ�ʼ����   //j=0�����
		q.pop();
		if (first)
		{
			printf("%s", &graph.vertices[tmpw].data.city);
			first = 0;
		}
		else
		{
			printf("->%s", &graph.vertices[tmpw].data.city);
		}
	}
	printf("\n");
}
void output(int v, Graph& graph, char kind)
{
	stack<int>q;//����Path[]��ÿ�����㴢���Ϊ��һ��������±꣬���½�һ��ջ��ջ��Ϊ�յ㣬ջ��Ϊ��� 
	int w = v;//�յ�
	while (Path[w] != -1)
	{
		q.push(w);
		w = Path[w];
	}
	q.push(w); //�����ѹ��ջ��
	while (q.size() != 1)
	{
		int w1 = q.top();//��j�αߵĿ�ʼ����   //j=0�����
		q.pop();
		int w2=q.top();
		ArcNode arc;
		GetArc(graph, w1, w2, arc, kind);
		printf("%s->%s\n", graph.vertices[w1].data.city,graph.vertices[w2].data.city);
		printf("Money��%.2lf\n", arc.money);
		printf("Time: %.2lf\n", arc.time);
		printf("Means of Transportation��%s\n", arc.kind);
		printf("\n");
	}
}
void output2(int v, Graph& graph, char Kind,char* kind)
{
	stack<int>q;//����Path[]��ÿ�����㴢���Ϊ��һ��������±꣬���½�һ��ջ��ջ��Ϊ�յ㣬ջ��Ϊ��� 
	int w = v;//�յ�
	while (Path[w] != -1)
	{
		q.push(w);
		w = Path[w];
	}
	q.push(w); //�����ѹ��ջ��
	while (q.size() != 1)
	{
		int w1 = q.top();//��j�αߵĿ�ʼ����   //j=0�����
		q.pop();
		int w2 = q.top();
		ArcNode arc;
		GetArc2(graph, w1, w2, arc, Kind,kind);
		printf("%s->%s\n", graph.vertices[w1].data.city, graph.vertices[w2].data.city);
		printf("Money��%.2lf\n", arc.money);
		printf("Time: %.2lf\n", arc.time);
		printf("Means of Transportation��%s\n", arc.kind);
		printf("\n");
	}
}
int main()
{
	vector<City> city;
	Graph graph;
	ShortPathTable Dist;
	char start[32] = { 0 };//������
	char des[32] = { 0 }; //Ŀ�ĵ�
	char Kind;//ѡ���Ȩֵ����,��T��Ϊʱ�䣬��M��Ϊ���� 
	char kind[6] = "\0";
	CreateCities("cities.csv", city);
	CreateGraph("routes.csv", graph, city);

	help();
	while (true)
	{
		int flag;
		printf("\n\n���������֣�");
		scanf_s("%d", &flag);
		switch (flag)
		{
		case 1:
		{
			printf("������������:");
			scanf_s("%s", &start, 32);
			char c = getchar();//��ȥ���з�  
			printf("�������յ����:");
			scanf_s("%s", &des, 32);
			c = getchar();
			printf("���������money������M�����������time������T:");
			scanf_s("%c", &Kind, 1);

			int v0 = SearchCity(start, city);//v0Ϊ����±�  
			int v = SearchCity(des, city);//vΪ�յ��±�
			Timer t;
			t.Start();
			ShortestPath(graph, v0, Dist, Kind);//�����·�� 
			t.Stop();
			printf(" Time elapsed is : %lf (ms). \n ", t.ElapsedTime());

			printf("The shortest path from %s to %s is:\n", start, des);
			outputPATH(v, graph);
			outputDist(v, Dist, Kind);
			printf("�Ƿ�鿴ÿ���ó���Ϣ��Y or N��");
			c = getchar();
			char option='O';
			scanf_s("%c", &option, 1);
			if (option == 'Y')
			{
				output(v, graph, Kind);
			}
			while (option != 'Y' && option != 'N')
			{
				printf("�Ƿ�鿴ÿ���ó���Ϣ��Y or N��");
				c = getchar();
				scanf_s("%c", &option, 1);
				if (option == 'Y')
				{
					output(v, graph, Kind);
				}
			}

			if (createhtml(graph, start, des, v, Kind))
			{
				printf("������ҳ�ɹ���\n");
				ShellExecuteA(NULL, "open", "chrome.exe", "tmp.html", NULL, SW_SHOWNORMAL);
			}
		}break;
		case 2:
		{
			printf("������������:");
			scanf_s("%s", &start, 32);
			char c = getchar();//��ȥ���з�  
			printf("�������յ����:");
			scanf_s("%s", &des, 32);
			c = getchar();
			printf("���������money������M�����������time������T:");
			scanf_s("%c", &Kind, 1);
			c = getchar();
			printf("�����뽻ͨ��ʽ:");
			scanf_s("%s", &kind, 6);

			int v0 = SearchCity(start, city);//v0Ϊ����±�  
			int v = SearchCity(des, city);//vΪ�յ��±�
			Timer t;
			t.Start();
			ShortestPath2(graph, v0, Dist, Kind, kind);//�����·�� 
			t.Stop();
			printf(" Time elapsed is : %lf (ms). \n ", t.ElapsedTime());
			printf("The shortest path from %s to %s is:\n", start, des);
			outputPATH(v, graph);
			outputDist(v, Dist, Kind);

			printf("�Ƿ�鿴ÿ���ó���Ϣ��Y or N��");
			c = getchar();
			char option = 'O';
			scanf_s("%c", &option, 1);
			if (option == 'Y')
			{
				output2(v, graph, Kind, kind);
			}
			while (option != 'Y' && option != 'N')
			{
				printf("�Ƿ�鿴ÿ���ó���Ϣ��Y or N��");
				c = getchar();
				scanf_s("%c", &option, 1);
				if (option == 'Y')
				{
					output2(v, graph, Kind, kind);
				}
			}
			if (createhtml(graph, start, des, v, Kind))
			{
				printf("������ҳ�ɹ���\n");
				ShellExecuteA(NULL, "open", "chrome.exe", "tmp.html", NULL, SW_SHOWNORMAL);
			}
		}break;
		case 3:
		{
			for (int i = 0; i <int(city.size()); i++)
			{
				city[i].pass = false;
			}
			printf("������������:");
			scanf_s("%s", &start, 32);
			int v0 = SearchCity(start, city);//v0Ϊ����±�  
			
			Timer t;
			t.Start();
			DFS(graph, v0, city);
			t.Stop();
			printf("\n Time elapsed is : %lf (ms). \n ", t.ElapsedTime());
		}break;
		case 4:
		{
			for (int i=0;i<int(city.size());i++)
			{
				city[i].pass = false;
			}
			printf("������������:");
			scanf_s("%s", &start, 32);
			int v0 = SearchCity(start, city);//v0Ϊ����±�  
			
			Timer t;
			t.Start();
			BFS(graph, v0, city);
			t.Stop();
			printf("\n Time elapsed is : %lf (ms). \n ", t.ElapsedTime());	
		}break;
		case 5:
		{
			help();
		}break;
		case 6: {
			return 0;
		}break;
		default:
			break;
		}
	}
	return 0;
}


