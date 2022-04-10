# save as a h.gp
# run : gnuplot h.gp


set palette model HSV functions gray,1,1
set cbrange [0:1]

# Set the output file type
set terminal png

# print to stdout
set print "-" 

# list of gradient names;  update it maually
# gnuplot array is numbered from 1 to words(array), not like c arrays
# update list with order as in function GiveColor from p.c
spaces = "RGB HSV"
sGradientGeneralType = "continous discrete"
sGradientJoiningType = "no  steps  tubes  diverging"



# length of array 
jMax = words(sGradientJoiningType)  
gMax = words(sGradientGeneralType) 


#  legend
set key inside bottom center horizontal
set key horizontal 
set key font ",8"

# remove upper and right axis
set border 3 back 
set xtics nomirror out
set ytics nomirror


set xlabel "gradient position"
set ylabel " intensity" 

# adjust y range to make a space for the legend 
set yrange [-0.2:1.1]

# recreate the names of txt files usnig the same algorithm as in FillArrayAndSaveTxtFile and SaveArray2PPM functions from h.c file : 
# FillArrayAndSaveTxtFile
h = 0.00
dh = 0.123

# for each input txt file with data create 2 output files ( png file with color profile) 
while (h<1.15) {

	do for [j=1:jMax]{ 
		do for [g=1:gMax]{ 

		
		# Set the intput file name
  		infile = sprintf('%f_%s_%s.txt', h, word(sGradientGeneralType, g), word(sGradientJoiningType, j))
  		print infile


		# first output file
		# Set the file name of the first file = rgb
  		outfile = sprintf('%f_%s_%s_rgb.png', h, word(sGradientGeneralType, g), word(sGradientJoiningType, j))
  		set output outfile
  		print outfile
  	 		

		# title of the image 
		sTitle = sprintf(" %s profiles of the pastel colormap with hue = %f", word(spaces, 1), h )
		set title sTitle
	
		
  		# Now plot the data with lines and points
		plot infile using 1:3 w lines linecolor rgb 'red' title 'R', \
     			''  using 1:4 w lines linecolor rgb 'green'  title 'G', \
     			''  using 1:5 w lines linecolor rgb 'blue'   title 'B',\
     			''  using 1:6 w lines linecolor rgb 'black'  title 'Y'
     		
     		
     		
     		# Set the intput file name
  		infile = sprintf('%f_%s_%s.txt', h, word(sGradientGeneralType, g), word(sGradientJoiningType, j))
  		print infile
	     		
		# second output file
  		# Set the file name of the second file = hsv
  		outfile = sprintf('%f_%s_%s_hsv.png', h, word(sGradientGeneralType, g), word(sGradientJoiningType, j))
  		set output outfile
  		print outfile
  	  		

		# title of the image 
		sTitle = sprintf(" %s profiles of the pastel colormap with hue = %f", word(spaces, 2), h )
		set title sTitle
	  	
  	
		# Now plot the data with lines and points
		plot infile using 1:7 w lines linecolor rgb 'red'   title 'H', \
     			''  using 1:8 w lines linecolor rgb 'green' title 'S', \
     		 	''  using 1:9 w lines linecolor rgb 'blue'  title 'V'
     	
     		}     	
     	}

	h = h + dh
	
}
     	
