

def unpickle(file):
    import pickle
    with open(file, 'rb') as fo:
        dict = pickle.load(fo, encoding='bytes')
    return dict


dict = unpickle("test_batch")
print(list(dict.keys()))
img_data = list(dict[b'data'])
input = []

for i in range(30, 31):
    # print(img_data)
    for j in range(len(img_data[i])):
        input += [img_data[i][j]]


print(input)
count = 0
with open('img_data.txt', 'w') as f:
    for number in input:
        if count % 12 == 0:
            f.write("\n")
        f.write(str(number) + ", ")
        count += 1
        
