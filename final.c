#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <ctype.h>
struct Huffman_Tree_Node
{
    int frequency;
    char character;
    struct Huffman_Tree_Node *Left;
    struct Huffman_Tree_Node *Right; 
};
int no_of_characters=0;
int count_insert=1;
struct code
{
    char character_code;
    char code_array[16];
    int code_length;
    
};
void Print_frequency(struct Huffman_Tree_Node* array[], FILE *output_file)
{
    for(int i=0;i<256;i++)
    {   
        if(array[i]->frequency!=0)
        {
            fprintf(output_file,"%c %d \n",array[i]->character,array[i]->frequency);
        }
    }
}
void Swap_Struct(struct Huffman_Tree_Node* struct1, struct Huffman_Tree_Node* struct2) {
    struct Huffman_Tree_Node temp = *struct1;
    *struct1 = *struct2;
    *struct2 = temp;
}
void Insert_Min_Heap(struct Huffman_Tree_Node* array[],int f,char c, struct Huffman_Tree_Node* left, struct Huffman_Tree_Node* right,int i)
{
    array[i]->frequency=f;
    array[i]->character=c;
    array[i]->Left=left;
    array[i]->Right=right;
    while(i>1)
    {
        int parent=(int)(i/2);
        if(array[parent]->frequency>array[i]->frequency)
        {
            Swap_Struct(array[parent],array[i]);
            i=parent;
        }
        else
        {
            count_insert++;
            return ;
        }
    }
    count_insert++;
}
void Print_Min_Heap_constructed(struct Huffman_Tree_Node* array[],FILE *output)
{
    for(int i=1;i<no_of_characters+1;i++)
    {
        fprintf(output,"%c %d\n",array[i]->character,array[i]->frequency);
    }
    fprintf(output,"\n");
}
void Print_Min_Heap_deleted(struct Huffman_Tree_Node* array[],FILE *output)
{
for(int i=1;i<no_of_characters;i++)
    {
        fprintf(output,"%c %d\n",array[i]->character,array[i]->frequency);
    }
    fprintf(output,"\n");
}

struct Huffman_Tree_Node* Extract_Min(struct Huffman_Tree_Node* array[],int* j)
{
    struct Huffman_Tree_Node* deleted_character = (struct Huffman_Tree_Node*)calloc(1, sizeof(struct Huffman_Tree_Node));

    deleted_character->frequency = array[1]->frequency;
    deleted_character->character = array[1]->character;
    deleted_character->Left = array[1]->Left;
    deleted_character->Right = array[1]->Right;

    Swap_Struct(array[1],array[*j]);

    array[*j]->frequency=999;
    array[*j]->character=27;
    int k=1;
    while(((2*k) + 1)<*j)
    {
        struct Huffman_Tree_Node* left=array[2*k];
        struct Huffman_Tree_Node* right=array[2*k+1];
        int larger;
        if(left->frequency<right->frequency)
        {
            larger=2*k;
        }
        else
        {
            larger=2*k+1;
        }

        if(array[k]->frequency>array[larger]->frequency)
        {
            Swap_Struct(array[k],array[larger]);
            k=larger;
        }
        else
        {
            (*j)--;
            return deleted_character;
        }
    }
    (*j)--;
    return deleted_character;
}
int elements_Huffman_Tree = 0;
struct Huffman_Tree_Node *Root;
struct Huffman_Tree_Node *Current;

struct Huffman_Tree_Node* create_new_node(int x,char c, struct Huffman_Tree_Node *left, struct Huffman_Tree_Node *right) {
    struct Huffman_Tree_Node *newnode = (struct Huffman_Tree_Node*)malloc(sizeof(struct Huffman_Tree_Node));
    newnode -> frequency = x;
    newnode->character=c;
    newnode -> Left=left;
    newnode -> Right=right;

    return newnode;
}
void Insert_Huffman_tree(struct Huffman_Tree_Node* array[], struct Huffman_Tree_Node* Root,struct Huffman_Tree_Node* node_1, struct Huffman_Tree_Node* node_2, int* j)
{   

    struct Huffman_Tree_Node* Left = create_new_node(node_1->frequency, node_1->character, node_1->Left, node_1->Right);
    struct Huffman_Tree_Node* Right = create_new_node(node_2->frequency, node_2->character, node_2->Left, node_2->Right);
    
    struct Huffman_Tree_Node* Root_node = create_new_node(node_1->frequency + node_2->frequency, 27, NULL, NULL);

    Root_node->Left = Left;
    Root_node->Right = Right;
    Root = Root_node;

    (*j)++;
    Insert_Min_Heap(array, Root_node->frequency, Root_node->character, Root_node->Left, Root_node->Right, *j);

    return;

}
int maxDepth(struct Huffman_Tree_Node* Root)
{
    if (Root == NULL)
        return 0;
    else 
    {
        int lDepth = maxDepth(Root->Left);
        int rDepth = maxDepth(Root->Right);
        if (lDepth > rDepth)
        return (lDepth + 1);
        else
        return (rDepth + 1);
    }
}

void Huffman_Codes(struct Huffman_Tree_Node* root, char path[], int depth, struct code codes[])
{
    if (root->Left == NULL && root->Right == NULL)
    {
        codes[root->character].character_code = root->character;
        codes[root->character].code_length = depth;
        for (int i = 0; i < depth; i++)
        {
            codes[root->character].code_array[i] = path[i];
        }
        return;
    }
    path[depth] = '0';
    Huffman_Codes(root->Left, path, depth + 1, codes);

    path[depth] = '1';
    Huffman_Codes(root->Right, path, depth + 1, codes);
}
void Print_Huffman_Code(struct code array[],FILE *output)
{
    for(int i=0;i<256;i++)
    {
        if(array[i].code_length>0)
        {
            fprintf(output,"%c -> ",array[i].character_code);
            for (int j = 0; j < array[i].code_length; j++)
            {
                fprintf(output,"%c", array[i].code_array[j]);
            }
        }
        fprintf(output,"\n");
    }
}
void Print_Huffman_Codes_Bitwise(FILE *input,struct code array[],FILE *output)
{
    unsigned char byte = 0;
    char input_character = 0;
    int Bit_Position = 7;
    fscanf(input,"%c",&input_character);
    while (feof(input) == 0) 
    {
        int i = 0;
        while(i < array[input_character].code_length)
        {
            while(Bit_Position >= 0)
            {
                if(array[input_character].code_array[i]=='1')
                {
                    byte |=(1 << Bit_Position);
                }
                Bit_Position--;
                i++;
                if(i == array[input_character].code_length){
                    if(feof(input) != 0){
                        break;
                    }
                    fscanf(input,"%c",&input_character);
                    i=0;
                }
            }

            fwrite(&byte, sizeof(unsigned char), 1, output);
            byte = 0;
            Bit_Position = 7;
        }
        fscanf(input,"%c",&input_character);
    }
}
void Decompress(FILE* input,struct Huffman_Tree_Node* root,FILE* output,int total_characters)
{
    unsigned char byte = 0;
    struct Huffman_Tree_Node *current=root;
    int i = 7;
    int count=0;
    fread(&byte, sizeof(byte), 1, input);
    while (feof(input) == 0) 
    {
        while(current->Left!=NULL && current->Right!=NULL)
        {
            unsigned char bit = (byte >> i) & 1;

            if(bit==0)
            {
                current=current->Left;
            }
            else
            {
                current=current->Right;
            }

            i--;

            if(i < 0 )
            {
                fread(&byte, sizeof(byte), 1, input);
                i = 7;
            }
        }
        count++;
        fprintf(output,"%c",current->character);
        current=root;
        if(count==total_characters)
        {
            return;
        }
       
    }
}


int main(int argc,char *argv[])
{
    FILE *fpinput;
    FILE *fpoutput;
    FILE *fp_Huffman_Tree_Node;
    FILE *fp_Min_Heap;
    FILE *fp_Compressed_File;
    FILE *fp_Characters_Codes;
    FILE *fp_Decompressed_File;
    fpinput=fopen(argv[1],"r");
    //fp_Huffman_Tree_Node=fopen("Huffman_Tree_Node.txt","w");
    //fp_Min_Heap=fopen("Min_Heap_Array.txt","w");
    if(fpinput== NULL)
    {
        printf("FILE NOT FOUND: %s\n", argv[1]);
        exit(2);
    }

    struct Huffman_Tree_Node* charArray[256] = {NULL};  // Array to store character frequency

    for(int i = 0; i < 256; i++){
        charArray[i] = (struct Huffman_Tree_Node*)calloc(1, sizeof(struct Huffman_Tree_Node));
    }   

    int ASCII_Character;

    // Read each character from the file
    while ((ASCII_Character = fgetc(fpinput)) != EOF)
    {
        // Increment the frequency of the character in the array
        charArray[ASCII_Character]->character = (char)ASCII_Character;
        charArray[ASCII_Character]->frequency++;
    }
    for(int i=0;i<256;i++)
    {   
        if(charArray[i]->frequency!=0)
        {
            no_of_characters++;
        }
    }
    rewind(fpinput);

    int count_delete = no_of_characters;

    //Print_frequency(charArray, fp_Huffman_Tree_Node);

   struct Huffman_Tree_Node** Min_Heap = (struct Huffman_Tree_Node**)calloc(no_of_characters+1, sizeof(struct Huffman_Tree_Node*));

    for(int i = 0; i<no_of_characters+1; i++){
        Min_Heap[i] = (struct Huffman_Tree_Node*)calloc(1, sizeof(struct Huffman_Tree_Node));
    }

   struct Huffman_Tree_Node* Huffman_Tree_Root = (struct Huffman_Tree_Node*)calloc(1, sizeof(struct Huffman_Tree_Node));

    if (Min_Heap == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(3);
    }

    for(int i=0;i<256;i++)
    {   
        if(charArray[i]->frequency!=0)
        {
            Insert_Min_Heap(Min_Heap,charArray[i]->frequency,charArray[i]->character, NULL, NULL, count_insert);
        }
    }
    //Print_Min_Heap_constructed(Min_Heap,fp_Min_Heap);
    while(count_delete != 1)
    {
        Insert_Huffman_tree(Min_Heap, Huffman_Tree_Root, Extract_Min(Min_Heap, &count_delete), Extract_Min(Min_Heap, &count_delete), &count_delete);
    }
    //Print_Min_Heap_deleted(Min_Heap,fp_Min_Heap);
    struct code codes[256];
    char path[16];
    for (int i=0;i<256;i++)
    {
        codes[i].character_code='\0';
        codes[i].code_array[0]='\0';
        codes[i].code_length=0;
    }
    memset(path, '\0', sizeof(path));
    struct Huffman_Tree_Node* Main_Root=Extract_Min(Min_Heap,&count_delete);
    Huffman_Codes(Main_Root, path, 0, codes);
    fp_Characters_Codes=fopen("Characters_Codes.txt","w");
    //Print_Huffman_Code(codes,fp_Characters_Codes);

    fp_Compressed_File = fopen("Compressed_File.bin", "wb");

    char input_character;
    Print_Huffman_Codes_Bitwise(fpinput,codes,fp_Compressed_File);
    fclose(fp_Compressed_File);
    fp_Compressed_File=fopen("Compressed_File.bin","rb");
    fp_Decompressed_File=fopen("Deompressed_File.txt","w");

    rewind(fpinput);
    int total_characters=0;
    char s;
    while ((s = fgetc(fpinput)) != EOF)
    {
        total_characters++;
    }
    //printf("%d",total_characters);
    fclose(fpinput);

    if (fp_Decompressed_File == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    Decompress(fp_Compressed_File,Main_Root,fp_Decompressed_File,total_characters);
    rewind(fp_Compressed_File);
    fclose(fp_Compressed_File);
    fclose(fp_Decompressed_File);
    fopen(argv[1], "rb");
    fseek(fpinput, 0, SEEK_END);
    float original_size = ftell(fpinput);
    printf("Original size : %.2f KB\n", original_size/1024);
    fclose(fpinput);
    fp_Compressed_File=fopen("Compressed_File.bin", "rb");
    fseek(fp_Compressed_File, 0, SEEK_END);
    float Reduced_size = ftell(fp_Compressed_File);
    printf("Final size : %.2f KB\n", Reduced_size/1024);
    fclose(fp_Compressed_File);
    float compression_percentage=((original_size-Reduced_size)/original_size)*100;
    printf("Compression : %.2f %%\n", compression_percentage);
    
}   