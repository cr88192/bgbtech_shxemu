#define BTESH2_EMLTE_TXPING			0x0000
#define BTESH2_EMLTE_TXPONG			0x0800
#define BTESH2_EMLTE_RXPING			0x1000
#define BTESH2_EMLTE_RXPONG			0x1800

#define BTESH2_EMLTE_MDIO_ADDR		0x07E4
#define BTESH2_EMLTE_MDIO_WRITE		0x07E8
#define BTESH2_EMLTE_MDIO_READ		0x07EC
#define BTESH2_EMLTE_MDIO_CTRL		0x07F0
#define BTESH2_EMLTE_PKTLEN			0x07F4
#define BTESH2_EMLTE_GIE			0x07F8
#define BTESH2_EMLTE_CTRL			0x07FC


u32 BTESH2_EmacLite_GetD(byte *buf,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	printf("EMAC Get\n");
	return(0);
}

int BTESH2_EmacLite_SetD(byte *buf,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	printf("EMAC Set\n");
	return(0);
}
