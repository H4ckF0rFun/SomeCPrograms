#include "BoxGame.h"
#include "Maps.h"
int main()

{
	BoxGame instance;
	instance.ShowTips();
	int i = 0;
	while (1)
	{
		int opt = instance.StartGame(maps[i].pSrcMap, maps[i].height, maps[i].width, i + 1, sizeof(maps) / sizeof(maps[0]));
		if (opt == 1)
			return 0;
		else if (opt == 2)
			continue;
		else if (opt == 3)
		{
			if (i > 0)
				i--;
		}

		else if (opt == 4)
		{
			if (i+1 < sizeof(maps) / sizeof(maps[0]))
				i++;
		}
	}
	
	return 0;
}