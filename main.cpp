#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <math.h>
using namespace std;
 
#define N 10
#define feature 4
vector< vector<string> > X;
string x[N][feature+1] = 
{
	{"��",    "��", "��", "��", "���ʺ�"},   
	{"��",    "��", "��", "��", "���ʺ�"},
	{"��",    "��", "��", "��",   "�ʺ�"},
	{"��",    "��", "��", "��",   "�ʺ�"},
	{"��",  "��ˬ", "��", "��",   "�ʺ�"},
	{"��",  "��ˬ", "��", "��", "���ʺ�"},
	{"��",  "��ˬ", "��", "��",   "�ʺ�"},
	{"��",    "��", "��", "��", "���ʺ�"},
	{"��",  "��ˬ", "��", "��",   "�ʺ�"},
	{"��",    "��", "��", "��",   "�ʺ�"},
//	{"��",    "��", "��", "��",   "�ʺ�"},
//	{"��",    "��", "��", "��",   "�ʺ�"},
//	{"��",    "��", "��", "��",   "�ʺ�"},
//	{"��",    "��", "��", "��", "���ʺ�"}
};
 
//�ĸ����������ƣ���������ȡֵ���������磬������ 
string attribute[] = {"����", "�¶�", "ʪ��", "�Ƿ��з�"};
 
vector<string> attributes;
 
//�������ݼ�
void createDataset() {
	//�������ݼ�
	X = vector< vector<string> >(N, vector<string>(feature+1));
	int i, j;
	for(i=0; i<N; i++) {
		for(int j=0; j<feature+1; j++) {
			X[i][j] = x[i][j];
		}
	}
	//��������
	for(i=0; i<feature; i++)
		attributes.push_back(attribute[i]);
}
 
//����������ݼ�����ũ��
double calcShanno(const vector< vector<string> > &data) {
	 int n = data.size();
	 map<string, int> classCounts;
	 int i;
	 int label = data[0].size() - 1;
    //��ʼΪ0
	 for(i=0; i<n; i++)
		classCounts[ data[i][label] ] = 0;
     //ÿ������һ�Σ�+1
	 for(i=0; i<data.size(); i++)
		classCounts[ data[i][label] ] += 1;
	 //������ũ��
	 double shanno = 0;
	 map<string, int>::iterator it;
	 for(it = classCounts.begin(); it != classCounts.end(); it++) {
		 double prob = (double)(it->second) / (double)n;
		 shanno -= prob * ( log(prob) / log(2) );
	 }
	 return shanno;
}
 
//���ո��������������ݼ������ֺ�����ݼ��в������������������µ����ݼ���ά������һ��
//axis �������±�
//value������ֵ
vector< vector<string> > splitDataSet(const vector< vector<string> > data, int axis, string value) {
	vector< vector<string> > result;
	for(int i=0; i<data.size(); i++) {
		if(data[i][axis] == value) {
			//������ǰ���������ά��ȥ��
			vector<string> removed(data[i].begin(), data[i].begin()+axis);
			removed.insert(removed.end(), data[i].begin()+axis+1, data[i].end());
			result.push_back(removed);
		}
	}
	return result;
}
 
//���������б�
vector<string> createFeatureList(const vector< vector<string> > &data, int axis) {
	int n = data.size();
	vector<string>featureList;   //����������ȡֵ
	set<string> s;
	for(int j=0; j<n; j++)    //Ѱ�Ҹ����������п���ȡֵ
		s.insert(data[j][axis]);
	set<string>::iterator it;
	for(it = s.begin(); it != s.end(); it++) {
		featureList.push_back(*it);
	}
	return featureList;
}
 
//ѡ����õ����ݼ����ַ�ʽ
int chooseBestFeatureToSplit(const vector< vector<string> > &data) {
	int n = data[0].size() - 1; 
	double bestEntropy = calcShanno(data);  //��ʼ��ũ��
	double bestInfoGain = 0;   //������Ϣ����
	int bestFeature = 0;       //��õ�����
    //��������
	for(int i=0; i<n; i++) {
		double newEntropy = 0;
        //�����������п���ȡֵ
		vector<string> featureList = createFeatureList(data, i);  
		for(int j=0; j<featureList.size(); j++) {
			vector< vector<string> > subData = splitDataSet(data, i, featureList[j]);
			double prob = (double)subData.size() / (double)data.size();
			newEntropy += prob * calcShanno(subData);   
		}
                          //��Ϣ���棬���صļ��٣�����������ȵļ���
		double infoGain = bestEntropy - newEntropy;  
		if(infoGain > bestInfoGain) {
			bestInfoGain = infoGain;
			bestFeature = i;
		}
	}
	return bestFeature;
}
 
//���س��ִ������ķ�������
//������ݼ��Ѵ������������ԣ������ǩ��Ȼ����Ψһ�ģ����ö�������ķ�������Ҷ�ӽڵ�ķ���
string majorityCnt(vector<string> &classList) {
	int n = classList.size();
	map<string, int> classCount;
	int i;
	for(i=0; i<n; i++)
		classCount[classList[i]] = 0;
	for(i=0; i<n; i++)
		classCount[classList[i]] += 1;
	int maxCnt = 0;
	map<string, int>::iterator it;
	string result = "";
	for(it = classCount.begin(); it != classCount.end(); it++) {
		if(it->second > maxCnt) {
			maxCnt = it->second;
			result = it->first;
		}
	}
	return result;
}
 
struct Node {
	string attribute; 
	string val; 
	bool isLeaf;
	vector<Node*> childs;
	Node() {
		val = "";
		attribute = "";
		isLeaf = false;
	}
};
Node *root = NULL;
 
//�ݹ鹹��������
Node* createTree(Node *root, const vector< vector<string> > &data, vector<string> &attribute) {
	if(root == NULL)
		root = new Node();
	vector<string> classList;
	set<string> classList1;
	int i, j;
	int label = data[0].size() - 1;
	int n = data.size();
	for(i=0; i<n; i++) {
		classList.push_back(data[i][label]);
		classList1.insert(data[i][label]);
	}
    //�������ʵ��������ͬһ�ֹ࣬ͣ����
	if(classList1.size() == 1) {
		if(classList[0] == "�ʺ�")
			root->attribute = "�ʺ�";
		else
			root->attribute = "���ʺ�";
		root->isLeaf = true;
		return root;
	}
    //�������������������س��ִ����������
	if(data[0].size() == 1) {
		root->attribute = majorityCnt(classList);
		return root;
	}
 
	int bestFeatureIndex = chooseBestFeatureToSplit(data);
    //�õ����Ե����п���ֵ
	vector<string> featureList = createFeatureList(data, bestFeatureIndex);  
	string bestFeature = attribute[bestFeatureIndex];
    //��¼Ҫ���ֵ�����
	root->attribute = bestFeature;   
    //���ڵ�ǰ���Ե�ÿ������ֵ�������µķ�֧
	for(i=0; i<featureList.size(); i++) {
		vector<string> subAttribute;  
		for(j=0; j<attribute.size(); j++) {
			if(bestFeature != attribute[j])
				subAttribute.push_back(attribute[j]);
		}
		Node *newNode = new Node();
		newNode->val = featureList[i];//��¼���Ե�ȡֵ
		createTree(newNode, splitDataSet(data, bestFeatureIndex, featureList[i]), subAttribute);
		root->childs.push_back(newNode);
	}
	return root;
}
 
//��ӡ
void print(Node *root, int depth) {
	int i;
	for(i=0; i<depth; i++)
		cout << "\t";
	if(root->val != "") {
		cout << root->val << endl;
		for(i=0; i<depth+1; i++)
			cout << "\t";
	}
	cout << root->attribute << endl;
	vector<Node*>::iterator it;
	for(it = root->childs.begin(); it != root->childs.end(); it++) {
		print(*it, depth+1);
	}
}
 
//Ԥ��x
string classify(Node *root, vector<string> &attribute, string *test) {
	string firstFeature = root->attribute;
	int firstFeatureIndex;
	int i;
    //�ҵ����ڵ��ǵڼ�������
	for(i=0; i<feature; i++) {
		if(firstFeature == attribute[i]) {
			firstFeatureIndex = i;
			break;
		}
	}
	if(root->isLeaf)  //�����Ҷ�ӽڵ㣬ֱ��������
		return root->attribute;
	for(i=0; i<root->childs.size(); i++) {
		if(test[firstFeatureIndex] == root->childs[i]->val) {
			return classify(root->childs[i], attribute, test);
		}
	}
}
 
//�ͷŽڵ�
void freeNode(Node *root) {
	if(root == NULL)
		return;
	vector<Node*>::iterator it;
	for(it=root->childs.begin(); it != root->childs.end(); it++)
		freeNode(*it);
	delete root;
}
 
int main() {	
	createDataset();
	root = createTree(root, X, attributes);
	print(root, 0);
	string test[] = {"��", "��", "��", "��"};
	int i;
	cout << endl << "���ԣ�";
	for(i=0; i<feature; i++)
		cout << attributes[i] << "\t";
	cout << endl << "���ӣ�";
	for(i=0; i<feature; i++)
		cout << test[i] << "\t";
	cout << endl << "Ԥ�⣺";
	cout << classify(root, attributes, test) << endl;
	freeNode(root);
	return 0;
}
