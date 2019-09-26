#pragma once

class cAseLoader;

typedef struct NODE
{
	int row, col, weight;

	float actualCostFromStart;
	float Heuristic;

	NODE * parent;

	bool inOpenSet, inCloseSet;

	NODE() : 
		weight(0), 
		actualCostFromStart(0),
		Heuristic(0), 
		parent(NULL), 
		inOpenSet(false), 
		inCloseSet(false)
	{}
};

class cPathFinder
{
public:
	cPathFinder();
	~cPathFinder();

public:
	void Setup(int row, int col, int cellwidth, int cellheight);
	void SetObstacle(int row, int col);

	void Find(int sx, int sy, int ex, int ey);

	void Play();
	void Stop();

	void Update(cAseLoader * player);
private:

	int												m_row, m_col;
	int												m_cellWidth, m_cellHeight;
	vector<vector<NODE>>							m_vecNodes;
	unordered_map<NODE*, NODE*>						m_path;
	NODE *											m_current;
	NODE *											m_start;
	bool											m_finish;

private:
	float ComputerHeuristic(int cx, int cy, int ex, int ey);
};

