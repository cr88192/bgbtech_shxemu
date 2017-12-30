#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv)
{
	int cr0, cg0, cb0, cy0, cu0, cv0;
	int cr1, cg1, cb1, cy1, cu1, cv1;
	int cr2, cg2, cb2, cy2, cu2, cv2;
	int cr3, cg3, cb3, cy3, cu3, cv3;
	int t;
	int i, j, k;

	for(i=0; i<16; i++)
	{
		cr0=rand()&255;		cg0=rand()&255;		cb0=rand()&255;
		cr1=rand()&255;		cg1=rand()&255;		cb1=rand()&255;
		
		cy0=(cr0+2*cg0+cb0)/4;
		cu0=(cb0-cg0)/2+128;
		cv0=(cr0-cg0)/2+128;

		cy1=(cr1+2*cg1+cb1)/4;
		cu1=(cb1-cg1)/2+128;
		cv1=(cr1-cg1)/2+128;

		t=64+(rand()&127);

		cr2=(cr0*(255-t)+cr1*t)/255;
		cg2=(cg0*(255-t)+cg1*t)/255;
		cb2=(cb0*(255-t)+cb1*t)/255;
		cy2=(cy0*(255-t)+cy1*t)/255;
		cu2=(cu0*(255-t)+cu1*t)/255;
		cv2=(cv0*(255-t)+cv1*t)/255;

//		cr2=(cr0+cr1)/2;	cg2=(cg0+cg1)/2;	cb2=(cb0+cb1)/2;
//		cy2=(cy0+cy1)/2;	cu2=(cu0+cu1)/2;	cv2=(cv0+cv1)/2;

		cu3=(cu2-128)<<1; cv3=(cv2-128)<<1;
		cg3=cy2-((cu3+cv3)>>2);
		cb3=cg3+cu3;
		cr3=cg3+cv3;

		printf("%d %d %d   %d %d %d\n", cr2, cg2, cb2, cr3, cg3, cb3);
	}
}