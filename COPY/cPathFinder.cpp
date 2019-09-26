#include "stdafx.h"
#include "cPathFinder.h"
#include "cAseLoader.h"


cPathFinder::cPathFinder()
{
}


cPathFinder::~cPathFinder()
{
}

void cPathFinder::Setup(int row, int col, int cellwidth, int cellheight)
{
	m_row		 = row;
	m_col		 = col;
	m_cellWidth  = cellwidth;
	m_cellHeight = cellheight;

	m_vecNodes.resize(col, vector<NODE>(row));
	m_start = NULL;
	m_finish = NULL;
}

void cPathFinder::SetObstacle(int row, int col)
{
	m_vecNodes[row][col].weight = 1000;
}

void cPathFinder::Find(int sx, int sy, int ex, int ey)
{
	vector<int> dirX = { -1, -1, -1, 0, 1, 1, 1, 0 };
	vector<int> dirY = { -1, 0, 1, 1, 1, 0, -1, -1 };

	vector<NODE*>	openSet;

	NODE * current = &m_vecNodes[sy][sx];
	NODE * goal = &m_vecNodes[ey][ex];

	current->row = sy;
	current->col = sx;
	current->inCloseSet = true;

	goal->row = ey;
	goal->col = ex;

	do
	{
		for (size_t i = 0; i < 8; ++i)
		{
			int x = dirX[i];
			int y = dirY[i];

			if (current->row + y < 0
				|| current->row + y >= m_vecNodes.size() ||
				current->col + x < 0
				|| current->col + x >= m_vecNodes.size())
				continue;


			NODE*	neighbor = &m_vecNodes[current->row + y][current->col + x];
			neighbor->row = current->row + y;
			neighbor->col = current->col + x;

			if (!neighbor->inCloseSet)
			{
				if (!neighbor->inOpenSet)
				{
					m_path[neighbor] = neighbor;
					m_path[neighbor]->parent = current;

					neighbor->Heuristic = ComputerHeuristic
					(current->col, current->row,
						goal->col, goal->row);
					neighbor->actualCostFromStart =
						current->actualCostFromStart +
						neighbor->weight + (sqrtf(x*x + y*y));

					neighbor->inOpenSet = true;
					openSet.push_back(neighbor);
				}
				else
				{
					float newG = current->actualCostFromStart + neighbor->weight + sqrt(x * x + y * y);

					if (newG < neighbor->actualCostFromStart)
					{
						m_path[neighbor]->parent = current;
						neighbor->actualCostFromStart = newG;
					}
				}
			}
		}
			if (openSet.empty())
				break;

			auto it =
				min_element(openSet.begin(), openSet.end(),
					[](const NODE*lhs, const NODE* rhs)
			{
				return lhs->Heuristic +
					lhs->actualCostFromStart
					<
					rhs->Heuristic +
					rhs->actualCostFromStart;
			});

			current = *it;
			openSet.erase(it);
			current->inOpenSet = false;
			current->inCloseSet = true;
		}

		while (current != goal);

			if (current == goal) m_start = goal;
}

void cPathFinder::Play()
{
	m_current = m_start;
	m_finish = false;
}

void cPathFinder::Stop()
{
	m_finish = true;
}

void cPathFinder::Update(cAseLoader * player)
{
	if (m_finish)
		return;

	if (m_current)
	{
		//D3DXVECTOR3 position = player->GetTransform()->GetPosition();
		//D3DXVECTOR3 dest(
		//	(-(m_cellWidth * m_col) + 1) + (m_current->col * m_cellWidth * 2.0f),
		//	position.y,
		//	(-m_cellHeight * m_row + 1) + (m_current->row * m_cellHeight * 2.0f));

		//D3DXVECTOR3 direction = dest - position;
		//float dist = D3DXVec3Length(&direction);

		//player->Move(direction / dist * 0.01f);

		//if (dist <= 0.15f)
		//	m_current = m_current->parent;
	}
	else m_finish = true;
}

float cPathFinder::ComputerHeuristic(int cx, int cy, int ex, int ey)
{
	return sqrt((cx - ex) * (cx - ex) + (cy - ey) * (cy - ey));
}
