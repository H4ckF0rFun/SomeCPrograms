#include "ChemistryEquationStringProcess.h"
#include "EquationSetSolve.h"

int main(int argc,char* argv[])
{
	SUBSTANCEPOINTER*pSubsAddr;
	char szRet[256] = {0};
	char buff[32];
	Unknown*uk;
	int MinCommonMultiple = 0;

	if(argc == 1)
	{
		printf("<ChemistryEquation> <ShowProcess(1(yes)/0(no))>\n");
		return 0;
	}

	if(argc == 3)
		StrToNum(argv[2],&bShowProcess);
	if(FALSE == ChemistryEquationStringProcess(argv[1]))
	{
		printf("Invalid Chemistry Equation!\n");
		printf("%s\n",szRetEquation);
		return 0;
	}
	if(FALSE == EquationSetSolve(szEquationSet))
	{
		printf("Sorry,this chemistry equation doesn't have solution!\n");
		return 0;
	}
	//----------------------ȡ��ĸ��С������---------------------;
	for(uk = RetSolution->unknown;uk!=NULL;uk=uk->next)
	{
		if(MinCommonMultiple == 0)
			MinCommonMultiple = uk->value.Denominator;
		else
			MinCommonMultiple = GetMinCommonMultiple(uk->value.Denominator,MinCommonMultiple);
	}
	//----------------------ͨ��-----------------------------;
	for(uk = RetSolution->unknown;uk!=NULL;uk=uk->next)
		uk->value.Molecule*=MinCommonMultiple/uk->value.Denominator;
	//----------------------------���ɽ��--------------------------;
	for(pSubsAddr = AllSubsAddrsHead;pSubsAddr!=NULL;pSubsAddr=pSubsAddr->next)
	{
		Unknown*uk;
		for(uk = RetSolution->unknown;uk!=NULL;uk=uk->next)
		{
			if(!strcmp(uk->Name,pSubsAddr->pAddress->kName))
			{
				if(uk->value.Molecule!=1)
				{
					sprintf(buff,"%d",uk->value.Molecule);
					strcat(szRet,buff);
				}
				strcat(szRet,pSubsAddr->pAddress->Name);
				if(pSubsAddr->pAddress->next != NULL && pSubsAddr->next->pAddress != RightSubsHead)
					strcat(szRet,"+");
				if(pSubsAddr->next!=NULL &&pSubsAddr->next->pAddress == RightSubsHead)
					strcat(szRet,"===");
			}
		}
	}
	//--------------------------������--------------------------;
	printf(szRet);
	//�ͷ��ڴ�;
	EndChemistryEquationStringProcess();
	EndEquationSetSolve();
	return 0;
}