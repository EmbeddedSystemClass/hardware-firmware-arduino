#ifndef _WEBPAGE_H
	#define _WEBPAGE_H

const PROGMEM char Page1[] = {
"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\r\n"
"<html>\r\n"
"<head>\r\n"
"  <title>SVG Graphs example</title>\r\n"

"  <style type='text/css'>\r\n"
"  body {\r\n"
"    background-color: #ffffff;\r\n"
"  }\r\n"

"  svg.graph {\r\n"
"    height: 500px;\r\n"
"    width: 1100px;\r\n"
"  }\r\n"

"  svg.graph .grid {\r\n"
"    stroke: white;\r\n"
"    stroke-dasharray: 1 2;\r\n"
"    stroke-width: 1;\r\n"
"  }\r\n"

"  svg.graph .points {\r\n"
"    stroke: white;\r\n"
"    stroke-width: 3;\r\n"
"  }\r\n"
"  svg.graph .first_set {\r\n"
"    fill: #000000;\r\n"
"  }\r\n"

"  svg.graph .surfaces {\r\n"
"    fill-opacity: 0.5;\r\n"
"  }\r\n"

"  svg.graph .grid.double {\r\n"
"    stroke-opacity: 0.4;\r\n"
"  }\r\n"

"  svg.graph .labels {\r\n"
"    font-family: Arial;\r\n"
"    font-size: 14px;\r\n"
"    kerning: 1;\r\n"
"  }\r\n"

"  svg.graph .labels.x-labels {\r\n"
"    text-anchor: middle;\r\n"
"  }\r\n"

"  svg.graph .labels.y-labels {\r\n"
"    text-anchor: end;\r\n"
"  }\r\n"
"  </style>\r\n"

"</head>\r\n"

"<body>\r\n"
"  <svg class='graph' version='1.1' xmlns:xlink='http://www.w3.org/1999/xlink' xmlns='http://www.w3.org/2000/svg'>\r\n"
"    <g class='grid x-grid' id='xGrid'>\r\n"
"    	%CHART_VERTICAL_LINES"  
"    </g>\r\n"
"    <g class='grid y-grid' id='yGrid'>\r\n"
"    	%CHART_HORIZONTAL_LINES"
"    </g>\r\n"


"    <g class='surfaces'>\r\n"
"        <path class='first_set' d=' %CHART_PATH Z'></path>\r\n"
"    </g>\r\n"

"    <use class='grid double' xlink:href='#xGrid' style=''></use>\r\n"
"    <use class='grid double' xlink:href='#yGrid' style=''></use>\r\n"

"    <g class='first_set points' data-setname='Our first data set'>\r\n"
"    	%CHART_POINTS" 
"    </g>\r\n"


"    <g class='labels x-labels'>\r\n"
"    	%CHART_XLABELS" 
"    </g>\r\n"
"    <g class='labels y-labels'>\r\n"
"    	%CHART_YLABELS" 
"    </g>\r\n"
"  </svg>\r\n"
"</body>\r\n"
"</html>\r\n"
"%END"
};

const PROGMEM char Circle[] = {
  "<circle cx='????' cy='????' data-value='7.2' r='5'></circle>"
};

#endif //_WEBPAGE_H
