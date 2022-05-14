#include <iostream>
#include <vector>

struct internalnode
{
	char data;
	unsigned int  sum;
	struct internalnode *left, *right;
};

struct keyvaluepair
{
	char data;
	unsigned int  frequency;
};

struct min1min2
{
	int min1;
	int min2;
};

std::vector< int > arr;
std::vector< int > leafnodes[10];
int leafnodescount = 0;


void initialize_the_nodes(internalnode* internal_nodes, keyvaluepair* kv, int size)
{
	for (int i = 0; i < size; i++)
	{
		internal_nodes[i].data = kv[i].data;
		internal_nodes[i].sum = kv[i].frequency;
		internal_nodes[i].left = NULL;
		internal_nodes[i].right = NULL;
	}
}

min1min2 findmin1min2(keyvaluepair* kv, int size)
{
	int i, first, second, first_index, second_index;

	first = second = INT_MAX;
	second_index = first_index = 0; 
	for (i = 0; i < size; i++)
	{
		if (kv[i].frequency <= first)
		{
			second = first;
			second_index = first_index;
			first = kv[i].frequency;
			first_index = i;
		}

		else if (kv[i].frequency < second)
		{
			second = kv[i].frequency;
			second_index = i;
		}
	}

	return{ first_index, second_index };
}

void printLeafNodes(internalnode *root)
{
	if (!root)
		return;

	// if node is leaf node, print its data   
	if (root->left == NULL && root->right == NULL)
	{
		leafnodescount = leafnodescount + 1;
		std::cout << root->data << " " << root->sum << " ";
		for (int i = 0; i < arr.size(); i++)
		{
			std::cout << arr[i];
		}
		for (int j = 0; j < arr.size(); j++)
		{
			leafnodes[leafnodescount - 1].push_back(arr[j]);
		}
		leafnodes[leafnodescount - 1].push_back(root->data);
		std::cout << "\n";
		return;
	}

	// if left child exists, check for leaf
	// recursively
	if (root->left)
		arr.push_back(0);
		printLeafNodes(root->left);
		arr.pop_back();

	// if right child exists, check for leaf
	// recursively
	if (root->right)
		arr.push_back(1);
		printLeafNodes(root->right);
		arr.pop_back();
}

void encode(int size)
{
	int max_charcters;
	std::cout << "Enter the number of charcters to be encoded as..\n";
	std::cin >> max_charcters;
	std::cout << "Enter " << max_charcters << " charcters" << std::endl;
	char* input_string = (char*)malloc(max_charcters * sizeof(char));
	for (int c = 0; c < max_charcters; c++)
	{
		std::cin >> input_string[c];
	}
	for (int i = 0; i < max_charcters; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((int)input_string[i] == leafnodes[j][leafnodes[j].size() - 1])
				for (int k = 0; k < leafnodes[j].size() - 1; k++)
				{
					std::cout << leafnodes[j][k];
				}
		}
	}
}

void decode(int size)
{
	int max_charcters;
	std::cout << "Enter the number of bits to be decoded as characters..\n";
	std::cin >> max_charcters;
	std::cout << "Enter " << max_charcters << " bits" << std::endl;
	char* encoded_message = (char*)malloc(max_charcters * sizeof(char));
	for (int m = 0; m < max_charcters; m++)
	{
		std::cin >> encoded_message[m];
	}

	// decoding algorithm.
	for (int k = 0; k < max_charcters; k++)
	{
		for (int i = 0; i < size; i++)
		{
			bool broke = false;
			for (int j = 0; j < leafnodes[i].size() - 1; j++)
			{
				if (((int)encoded_message[k] - 48) == leafnodes[i][j])
				{
					k++;
					continue;
				}
				else
				{
					k = k - j;
					broke = true;
					break;
				}
			}
			if (broke == false)
			{
				std::cout << (char)leafnodes[i][leafnodes[i].size() - 1];
				k = k - 1;
				break;
			}
		}
	}
}

int main()
{
	int size = 0;
	std::cout << "Enter Number of characters to encode \n";
	std::cin >> size;
	keyvaluepair* m = (keyvaluepair*)malloc(((2*size) - 1) * sizeof(keyvaluepair));
	keyvaluepair* m1 = (keyvaluepair*)malloc(size * sizeof(keyvaluepair));
	internalnode* internal_nodes = (internalnode*)malloc(((2 * size - 1)) * sizeof(internalnode));

	std::cout << "Enter character and its frequency\nExample: x 5 is considered as one input\n";
	for (int i = 0; i < size; i++)
	{
		std::cin >> m[i].data;
		std::cin >> m[i].frequency;
		m1[i].data = m[i].data;
		m1[i].frequency = m[i].frequency;
	}
	for (int i = size; i < ((2 * size) - 1); i++)
	{
		m[i].data = 'q';
		m[i].frequency = INT32_MAX;
	}

	initialize_the_nodes(internal_nodes, m, (2 * size - 1));

	// finding the huffman tree.
	min1min2 m1m2;
	for (int i = size; i < (2*size - 1); i++)
	{
		m1m2 = findmin1min2(m, (2*size - 1));
		internal_nodes[i].sum = m[m1m2.min1].frequency + m[m1m2.min2].frequency;
		m[m1m2.min1].frequency = INT32_MAX;
		m[m1m2.min2].frequency = INT32_MAX;
		m[i].frequency = internal_nodes[i].sum;
		internal_nodes[i].left = &internal_nodes[m1m2.min1];
		internal_nodes[i].right = &internal_nodes[m1m2.min2];
	}

	internalnode* internal_node_base_address = internal_nodes;
	std::cout << "Assigning Efficient binary code to each character...." << std::endl;

	printLeafNodes(&internal_node_base_address[2*(size-1)]);

	std::cout << "\n................................................................\n";

	bool wrong_input = false;
	bool stop_the_program = false;
	while (true)
	{
		std::cout << "\n\npress e to encode the message, d to decode the message, q to quit the program." << std::endl;
		char input;
		std::cin >> input;
		switch (input)
		{
		case 'e':
			encode(size);
			break;

		case 'd':
			decode(size);
			break;

		case 'q':
			stop_the_program = true;
			break;

		default:
			std::cout << "You have pressed a wrong button! restert....\n";
			wrong_input = true;
			break;
		}
		if ((wrong_input == true) || (stop_the_program == true))
			break;
	}

	char exit_program;
	std::cout << "Enter any key to exit the program" << std::endl;
	std::cin >> exit_program;
	return 0;
}
