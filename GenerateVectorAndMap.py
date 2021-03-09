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


print()
list_number=['0p0', '1p0', '2p0', '3p0', '4p0', '5p0', '6p0', '7p0', '8p0', '9p0', \
        '10p0', '11p0', '12p0', '13p0', '14p0', '15p0', '16p0', '17p0', '18p0', '19p0', '20p0',\
        '-1p0', '-2p0', '-3p0', '-4p0', '-5p0', '-6p0', '-7p0', '-8p0', '-9p0', \
        '-10p0', '-11p0', '-12p0', '-13p0', '-14p0', '-15p0', '-16p0', '-17p0', '-18p0', '-19p0', '-20p0']
for number in list_number:

    number = number.replace('-', 'n')
    if number == '1p0':
        name = 'hhttbbKL' + number
        print('base_names.push_back(\"{}\");'.format(name))
        continue


    if number == '10p0':
        name = 'hhttbbKL' + number
        print('base_names.push_back(\"{}\");'.format(name))
        continue


    for fromwhat in ['from1p0', 'from10p0']:
        name = 'hhttbbKL' + number + fromwhat 
        print('base_names.push_back(\"{}\");'.format(name))

for number in list_number:

    print(number.replace('-', 'n') + ' ', end='')

print()
list_number=['1p0', '10p0','20p0']
for number in list_number:
    if number == '1p0':
        name = 'hhttbbKL' + number
        print('base_names.push_back(\"{}\");'.format(name))
        continue

    if number == '10p0':
        name = 'hhttbbKL' + number
        print('base_names.push_back(\"{}\");'.format(name))
        continue

    for fromwhat in ['from1p0', 'from10p0']:
        name = 'hhttbbKL' + number + fromwhat 
        print('base_names.push_back(\"{}\");'.format(name))



