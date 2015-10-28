#ifndef _WEBPAGE_H
	#define _WEBPAGE_H

const PROGMEM char Page1[] = {
"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\r\n"
"<html>\r\n"
"<head>\r\n"
"  <title>Temperature</title>\r\n"

"<script>\r\n"
"	var data = [300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300];\r\n"
"	function GetArduinoInputs()\r\n"
"	{\r\n"
	
"		var path = document.getElementById(\"temp-path\");\r\n"
	
"		for(var i = 0; i < 20; i++) {\r\n"
"			var circle = document.getElementById(\"dot\" + (\"0000\" + i).slice(-4));\r\n"
"			circle.setAttributeNS(null, \"cy\", data[i]);\r\n"
			
"			var segments = path.pathSegList;\r\n"
"			segments.getItem(i+1).y = parseInt(data[i]);\r\n"
"		}\r\n"
		
"		for(var i = 0; i < 19; i++) {\r\n"
"			data[i] = data[i + 1];\r\n"
"		}\r\n"
	
	
"		var request = new XMLHttpRequest();\r\n"
	
"		request.onreadystatechange = function()\r\n"
"			{\r\n"
"				if (this.readyState == 4) {\r\n"
"					if (this.status == 200) {\r\n"
"						if (this.responseXML != null) {\r\n"
"							var temp = this.responseXML.getElementsByTagName('analog')[0].childNodes[0].nodeValue;\r\n"
"							data[19] = temp * -2.5 + 250 + 50;\r\n"
"							document.getElementById('vname').value = temp;\r\n"
"						}\r\n"
"					}\r\n"
"				}\r\n"
"			}\r\n"
	
"		request.open(\"GET\", \"ajax_inputs\", true);\r\n"
"		request.send(null);\r\n"
"		setTimeout('GetArduinoInputs()', 10*60*1000);\r\n"
"	}\r\n"
"</script>\r\n"
	
"  <style type='text/css'>\r\n"
"  body {\r\n"
"    background-color: #b8b8b8;\r\n"
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

"<body onload=\"GetArduinoInputs()\">\r\n"
"  <svg class='graph' version='1.1' xmlns:xlink='http://www.w3.org/1999/xlink' xmlns='http://www.w3.org/2000/svg'>\r\n"
"    <g class='grid x-grid' id='xGrid'>\r\n"
"    	%CHART_VERTICAL_LINES"  
"    </g>\r\n"
"    <g class='grid y-grid' id='yGrid'>\r\n"
"    	%CHART_HORIZONTAL_LINES"
"    </g>\r\n"


"    <g class='surfaces'>\r\n"
"        <path id = 'temp-path' class='first_set' d=' %CHART_PATH Z'></path>\r\n"
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

"  <form method = 'get' action='file.txt'>\r\n"
"    <p/>\r\n"
"    <input name=\"SUB\" value=\"Start Log\" type=\"submit\">\r\n"
"    <input name=\"SUB\" value=\"Stopp Log\" type=\"submit\">\r\n"
"    <input name=\"OUT\" type=\"radio\" value=\"C\" %LOGSTATE>"
"    <p/>\r\n"
"    <select name='download' size='5' style='width: 300px'>\r\n"
"       %FILES\r\n"
"    </select>\r\n"
"    <p/>\r\n"
"    <input name=\"SUB\" value=\"DownLoad\" type=\"submit\" >\r\n"
"  </form>\r\n"

"  <label for=\"vname\">Temp:\r\n"
"    <input id=\"vname\" name=\"vname\">\r\n"
"  </label>\r\n"

"</body>\r\n"
"</html>\r\n"
"%END"
};

const PROGMEM char Page2[] = {
"1000\r\n"
"2000\r\n"
"3000\r\n"
"4000\r\n"
"5000\r\n"
"6000\r\n"
"7000\r\n"
"8000\r\n"
"9000\r\n"
"10000\r\n"
"%END"
};

#endif //_WEBPAGE_H
