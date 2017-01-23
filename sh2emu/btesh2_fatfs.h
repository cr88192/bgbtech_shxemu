#define TKFAT_SFL_DIRTY		0x00000200

typedef struct BTESH2_TKFAT_MBR_Entry_s BTESH2_TKFAT_MBR_Entry;
typedef struct BTESH2_TKFAT_MBR_s BTESH2_TKFAT_MBR;
typedef struct BTESH2_TKFAT_FAT16_Boot_s BTESH2_TKFAT_FAT16_Boot;
typedef struct BTESH2_TKFAT_FAT32_Boot_s BTESH2_TKFAT_FAT32_Boot;
typedef struct BTESH2_TKFAT_FAT_DirEnt_s BTESH2_TKFAT_FAT_DirEnt;
typedef struct BTESH2_TKFAT_FAT_DirLfnEnt_s BTESH2_TKFAT_FAT_DirLfnEnt;

typedef struct BTESH2_TKFAT_FAT_DirEntExt_s BTESH2_TKFAT_FAT_DirEntExt;
typedef struct BTESH2_TKFAT_FAT_DirInfo_s BTESH2_TKFAT_FAT_DirInfo;

struct BTESH2_TKFAT_MBR_Entry_s {
	byte flag;				//0x80|=active
	byte shead;				//starting head
	byte ssect[2];			//starting track/sector
	byte fstype;			//filesystem
	byte ehead;				//ending head
	byte esect[2];			//ending track/sector
	byte lba_start[4];		//LBA start
	byte lba_count[4];		//LBA count
};

struct BTESH2_TKFAT_MBR_s {
	byte pad[446];		//MBR boot code
	BTESH2_TKFAT_MBR_Entry entry[4];
	byte aa55[2];		//should be 0xAA55
};

struct BTESH2_TKFAT_FAT16_Boot_s {
	byte pad_jmp[3];			//0x00, boot JMP
	byte oem_name[8];			//0x03
	byte bytes_sector[2];		//0x0B
	byte sectors_cluster;		//0x0D
	byte reserved_sectors[2];	//0x0E
	byte num_fats;				//0x10
	byte root_dirents[2];		//0x11
	byte lba_count16[2];		//0x13
	byte media_type;			//0x15
	byte sectors_fat[2];		//0x16
	byte sectors_track[2];		//0x18
	byte heads[2];				//0x1A
	byte hidden_sectors[4];		//0x1C
	byte lba_count[4];			//0x20
	byte drive_id;				//0x24
	byte drive_flag;			//0x25
	byte ebsig;					//0x26
	byte vol_sn[4];				//0x27
	byte vol_label[11];			//0x2B
	byte fs_tyname[8];			//0x36
	byte boot_code[448];		//0x3E
	byte aa55[2];				//0x1FE, should be 0xAA55	
};

struct BTESH2_TKFAT_FAT32_Boot_s {
	byte pad_jmp[3];			//0x00, boot JMP
	byte oem_name[8];			//0x03
	byte bytes_sector[2];		//0x0B
	byte sectors_cluster;		//0x0D
	byte reserved_sectors[2];	//0x0E
	byte num_fats;				//0x10
	byte root_dirents[2];		//0x11
	byte lba_count16[2];		//0x13
	byte media_type;			//0x15
	byte sectors_fat[2];		//0x16
	byte sectors_track[2];		//0x18
	byte heads[2];				//0x1A
	byte hidden_sectors[4];		//0x1C
	byte lba_count[4];			//0x20
	byte sectors_fat32[4];		//0x24
	byte drive_flag2[2];		//0x28
	byte version[2];			//0x2A
	byte root_cluster[4];		//0x2C
	byte fsis_sector[2];		//0x30
	byte fsaltboot_sector[2];	//0x32
	byte resv[12];				//0x34
	byte drive_id;				//0x40
	byte drive_flag;			//0x41
	byte ebsig;					//0x42
	byte vol_sn[4];				//0x43
	byte vol_label[11];			//0x47
	byte fs_tyname[8];			//0x52
};

union BTESH2_TKFAT_FAT_Boot_s {
struct BTESH2_TKFAT_FAT16_Boot_s fat16;
struct BTESH2_TKFAT_FAT32_Boot_s fat32;
};

struct BTESH2_TKFAT_FAT_DirEnt_s {
	byte name[8];				//0x00
	byte ext[3];				//0x08
	byte attrib;				//0x0B
	byte lncase;				//0x0C
	byte ctime_ms;				//0x0D
	byte ctime[2];				//0x0E
	byte cdate[2];				//0x10
	byte mdate[2];				//0x12
	byte cluster_hi[2];			//0x14
	byte lmtime[2];				//0x16
	byte lmdate[2];				//0x18
	byte cluster_lo[2];			//0x1A
	byte filesize[4];			//0x1C
};

struct BTESH2_TKFAT_FAT_DirLfnEnt_s {
	byte seq;					//0x00
	byte name1[10];				//0x01
	byte attrib;				//0x0B
	byte type;					//0x0C
	byte csum;					//0x0D
	byte name2[12];				//0x0E
	byte cluster_lo[2];			//0x1A
	byte name3[4];				//0x1C
};

typedef struct BTESH2_TKFAT_ImageInfo_s BTESH2_TKFAT_ImageInfo;
typedef struct BTESH2_TKFAT_SegmentInfo_s BTESH2_TKFAT_SegmentInfo;

struct BTESH2_TKFAT_ImageInfo_s {
// byte *pImgData;
s64 nImgBlks;

FILE *fdImgData;

byte fsty;			//filesystem type
bool isfat16;		//FS is FAT16
bool isfat32e;		//FS is FAT32+
byte szclust;		//sectors/cluster
byte shclust;		//shift for cluster (bytes)

int lba_start;		//LBA start of FAT volume
s64 lba_count;		//LBA count of FAT volume

int lba_fat1;		//LBA of first FAT
int lba_fat2;		//LBA of second FAT
int lba_root;		//LBA of root directory
int lba_data;		//LBA of data start

int tot_clust;
int clid_root;

BTESH2_TKFAT_MBR *mbr;
BTESH2_TKFAT_FAT16_Boot *boot16;
BTESH2_TKFAT_FAT32_Boot *boot32;

u32 sbc_lba[64];
void *sbc_buf[64];
short sbc_lbn[256];
int sbc_num;

u32 tbc_lba[256];
short tbc_lbn[256];
void *tbc_buf[256];
int tbc_num;

char *exp_iname[256];
char *exp_ename[256];
int exp_cnt;

char *seg_base;
//FILE *seg_fd[256];
//u32 seg_id[256];
BTESH2_TKFAT_SegmentInfo *seg[256];
int seg_n;
};

struct BTESH2_TKFAT_SegmentInfo_s {
FILE *fd;
u32 id;
u32 idx[256];
int nidx;
};

struct BTESH2_TKFAT_FAT_DirEntExt_s {
BTESH2_TKFAT_FAT_DirEnt deb;	//basic dirent
BTESH2_TKFAT_ImageInfo *img;
int clid;				//cluster ID of parent directory
int idx;				//index within directory
byte de_name[512];		//name (UTF-8, LFN)
byte de_aname[512];		//name (UTF-8, LFN)
};

struct BTESH2_TKFAT_FAT_DirInfo_s {
BTESH2_TKFAT_ImageInfo *img;
int clid;			//cluster ID of current directory
};

int BTESH2_TKFAT_ReadSectors(BTESH2_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num);
int BTESH2_TKFAT_WriteSectors(BTESH2_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num);

int BTESH2_TKFAT_SetupDirEntNewDirectory(
	BTESH2_TKFAT_FAT_DirEntExt *dee);
