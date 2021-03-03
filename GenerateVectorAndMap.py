# Author: Zifeng Xu
# Only use to Generate My Code
# Warning: This is a Python3 script!
with open('variables_rebinfactor.txt', 'r') as file_object:
    # Generate for variables vector
    lines = file_object.readlines()
    for line in lines:
        index_1 = line.find('\"')
        index_2 = line[index_1 + 1:].find('\"') + index_1
        #print(index_1)
        #print(index_2)
        print('variables.push_back(\"{}\");'.format(line[index_1 + 1:index_2 + 1]))
    print()
    for line_2 in lines:
        index_1 = line_2.find('\"')
        index_2 = line_2[index_1 + 1:].find('\"') + index_1
        number = line_2[-5:-3]
        print('rebin_factors.insert(std::pair<std::string, int>(\"{1}\", {0}));'.format(number, line_2[index_1 + 1: index_2 + 1]))



list_number=['1p0', '2p0', '3p0', '5p0', '10p0', '20p0', '0p0', 'n1p0', 'n3p0', 'n4p0', 'n5p0', 'n10p0', 'n15p0']
for fromwhat in ['from1p0', 'from10p0']:
    for number in list_number:
        if(number == '10p0'):
            name = 'hhttbbKL' + number
        else:
            if(number == '1p0'):
                name = 'hhttbbKL' + number
            else:
                name = 'hhttbbKL' + number + fromwhat 
        print('base_names.push_back(\"{}\");'.format(name))
