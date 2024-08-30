from PIL import Image


character_amount = 30
scale = 1


img = Image.open('new_project.png')

width, height = img.size

x2, y2 = -1, -1

for i in range(character_amount):
    for y in range(8):
        y2 += 1
        if y2 % scale != 0:
            continue

        print('0b', end='')

        for x in range(width // character_amount):
            x2 += 1
            if x2 % scale != 0:
                continue

            rgb = img.getpixel((x + ((width / character_amount) * i), y))
            

            if rgb[0] < 100:
                print("1", end='')
            else:
                print("0", end='')
        print(', ', end='')
        # print()
    print()