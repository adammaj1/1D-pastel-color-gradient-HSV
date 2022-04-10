
# theory
[Pastel color](https://en.wikipedia.org/wiki/Pastel_(color))

# Name
* tints (a mixture of a base color with white )
* pale colors = increased lightness
* pastel colors
* soft or muted type of color
* light color


# Effect
* soothing to the eye
* looks less intense, pastel, pale, faded look.
* subtle, modern or sophisticated design


# Algorithm
* mix base color with white = heavily tinted with white = desaturated with white
* [first generate a random color. Then we sature it a little and mix this color with white color](https://mdigi.tools/random-pastel-color/ random-pastel-color by Micro Digital Tools)
* in HSV. Take a hue. Desaturate the color a bit( = 80% saturation). Use 100% for value. 
* in the HSV color space, have high value and low saturation








# git

## init
```git
echo "# 1D-pastel-color-gradient-HSV" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:adammaj1/1D-pastel-color-gradient-HSV.git
git push -u origin main# 1D-pastel-color-gradient-HSV
```



## Subdirectory
```git
mkdir images
git add *.png
git mv  *.png ./images
git commit -m "move"
git push -u origin main
then link the images:
```

  ![](./images/n.png "description") 

to overwrite: 

  git mv -f 


