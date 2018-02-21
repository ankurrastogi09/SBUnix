#include <stdio.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/kprintf.h>

uint64_t* kmalloc(size_t size);
uint64_t calNextFileAddress(unsigned char* size);

struct tarfs_node* tarfs_root;
struct tarfs_node* current_node;

char dirPath[10][128];
char curDirPath[10][128];


int memcmp_tds(const void* str1, const void* str2, size_t size){
	const unsigned char *lstr1 = str1, *lstr2 = str2;

	while(size--){
	  if(*lstr1 != *lstr2){
	    return *lstr1 - *lstr2;
	  } else{
	    lstr1++, lstr2++;
	  }
	}

	return 0;
}

int strlen_tds(char str[128]){
	int i = 0;
	for(i = 0; i < 128; i++){
		if(str[i] == '\0'){
			break;
		}
	}
	return i;
}

void tarfsDirStrInit(){
	tarfs_root = (struct tarfs_node*) kmalloc(1);
	for(int i = 0; i < TARFS_CHILDREN_NODE_LIMIT; i++){
		tarfs_root->childrens[i] = NULL;
	}
	tarfs_root->name[0] = '/';
	tarfs_root->type = TARFS_CHILDREN_FOLDER_TYPE;
	tarfs_root->size = 0;

	current_node = tarfs_root;
}

struct tarfs_node* tarfsDirGetNode(char* name, int type, int size){
	struct tarfs_node* child = (struct tarfs_node*) kmalloc(1);
	
	for(int i = 0; i < TARFS_CHILDREN_NODE_LIMIT; i++){
		child->childrens[i] = NULL;
	}
	for(int i = 0; i < 128; i++){
		child->name[i] = name[i];
	}
	//child->name = name;
	child->type = type;
	child->size = size;
	return child;
}

void tarfsDirNodeInsert(struct tarfs_node* node, char* name, int type, int size){
	for(int i = 0; i < TARFS_CHILDREN_NODE_LIMIT; i++){
		if(node->childrens[i] == NULL){
			node->childrens[i] = tarfsDirGetNode(name, type, size);
		}
	}
}

int compare(char buf1[], char buf2[], int size){
	for(int i = 0; i < size; i++){
		if(buf1[i] != buf2[i]){
			return 0;
		}
	}
	return 1;
}

void addNodeToFileStr(struct tarfs_node* node, int index, int type, int size){

	int matchingFlag = 0;
	//int notMatchinfFlag = 0;
	int nullNodeFound = 0;

	int i = 0;
	// for(i = 0; i < 128; i++){
	// 	if(node->childrens[i] == NULL){
	// 		notMatchinfFlag = 1;
	// 		break;
	// 	}
	// 	if(compare(node->childrens[i],dirPath[index],128) == 1){
	// 		matchingFlag = 1;
	// 		break;
	// 	}
	// }

	for(i = 0; i < 128; i++){
		if(node->childrens[i] == NULL){
			nullNodeFound = 1;
			break;
		}
		if(compare(node->childrens[i]->name,dirPath[index],128) == 1){
			matchingFlag = 1;
			break;
		}
	}

	if(nullNodeFound == 1){
		node->childrens[i] = tarfsDirGetNode(dirPath[index], type, size);
	}

	if(matchingFlag == 1){
		addNodeToFileStr(node->childrens[i], index + 1, type, size);
	}



	// int nodeDontHasChildFlag = 0;

	// if(dirPath[index][0] == '\0'){
	// 	return;
	// }
	// int i = 0;
	// for(i = 0; i < TARFS_CHILDREN_NODE_LIMIT; i++){
	// 	if(node->childrens[i]->name == NULL){
	// 		break;
	// 	} else{
	// 		int j = 0;
	// 		while(dirPath[index][j] != '\0'){
	// 			if(dirPath[index][j] != node->childrens[i]->name[j]){
	// 				nodeDontHasChildFlag = 1;
	// 			}
	// 		}	
	// 	}
	// }

	// if(nodeDontHasChildFlag == 0){
	// 	//if node is present	
	// 	addNodeToFileStr(node->childrens[i], index + 1, type, size);
	// } else {
	// 	//if node is not present
	// 	node->childrens[i] = tarfsDirGetNode(dirPath[index], type, size);
	// 	//addNodeToFileStr(node->childrens[i], dirPath, index + 1, type, size);
	// }
}

void checkForAdditionOfNode(char* path, int type, int size){
	for(int i = 0; i < TARFS_MAX_PATH_DEPTH; i++){
		for(int j = 0; j < TARFS_FILE_FOLDER_MAX_NAME_LENGTH; j++){
			dirPath[i][j] = '\0';	
		}
	}

	int x = 0;
	int y = 0;
	int i = 0;
	while(*(path + i) != '\0'){
		dirPath[x][y] = *(path + i);
		i++;
		y++;
		if(*(path + i) == '/'){
			x++;
			y=0;
			i++;
		}
	}

	addNodeToFileStr(tarfs_root, 0, type, size);
}

void printCurrentDirectorChildrens(){
	//kprintf1("\n");
	for(int i = 0; i < 128; i++){
		if(current_node->childrens[i] != NULL){
			kprintf1("Name %s\n", current_node->childrens[i]->name);
		}
	}
}

void traverseAndChangeCurrDir(struct tarfs_node* node, int index){
	int i = 0;
	int hasNodeFlag = 0;
	for(i = 0; i< 128; i++){
		if(node->childrens[i] != NULL && compare(node->childrens[i]->name, curDirPath[index], 128) == 1){
			hasNodeFlag = 1;
			break;
		}
	}

	if(hasNodeFlag == 1){
		if(curDirPath[index+1][0] != '\0'){
			traverseAndChangeCurrDir(node->childrens[i], index + 1);	
		} else {
			current_node = node->childrens[i];
		}
	} else {
		kprintf1("Path is not valid!!!\n");
	}
}

void changeDirectory(char* path){

	if(*path == '/' && *(path + 1) == '\0'){
		current_node = tarfs_root;
		return;
	}

	for(int i = 0; i < TARFS_MAX_PATH_DEPTH; i++){
		for(int j = 0; j < TARFS_FILE_FOLDER_MAX_NAME_LENGTH; j++){
			curDirPath[i][j] = '\0';	
		}
	}

	int x = 0;
	int y = 0;
	int i = 0;
	while(*(path + i) != '\0'){
		curDirPath[x][y] = *(path + i);
		i++;
		y++;
		if(*(path + i) == '/'){
			x++;
			y=0;
			i++;
		}
	}
	traverseAndChangeCurrDir(tarfs_root, 0);
}

// void traverseAndBuildTarfsDirStructure(){
// 	uint64_t startAddress = (uint64_t) &_binary_tarfs_start;
//         struct posix_header_ustar* phu;
	
// 	//Traversing over all the files in tarfs
//         while(startAddress < (uint64_t) &_binary_tarfs_end){
//                 phu = (struct posix_header_ustar*) startAddress;
//                 kprintf("abcd name %s size %s type %s linkname %s magic %s \n", phu->name, phu->size, phu->typeflag, phu->linkname, phu->magic);
                
//                 //kprintf1("Names are %s \n", phu->name);
//                 // if(memcmp_su(phu->name, pathname, strlen_su(pathname)) == 0){
//                 //         //kprintf1("file name is %s\n", phu->name);
//                 //         uint64_t readableFileStart = startAddress + sizeof(struct posix_header_ustar);
//                 //         //char* readableFilePointer = (char*) readableFileStart;
//                 //         // int i = 0;
//                 //         //kprintf1("%s \n", readableFilePointer);
//                 //         //return populateFDs(readableFileStart, pcb);
//                 // }

// 		startAddress += calNextFileAddress((unsigned char*)phu->size);
//         }
// }

