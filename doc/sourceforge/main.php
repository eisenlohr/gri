<?php
require("subroutines.php");
set_up_navigation_tabs("main", "main.png");
set_up_lhs();
print "<b>G r i</b><br><br><br>\n";
print "<b>V e r s i o n</b><br><br><br>\n";
print "<b>$gri_stable_version</b><br><br><br><br><br>\n";
set_up_rhs();
?>

<!--
<h1>Version</h1>
<p>
The present stable version is numbered <?php print $gri_stable_version; ?>.
-->

<h1>What Gri is</h1>

<p>
Gri is a language for scientific graphics programming.  The word
"language" is important: Gri is command-driven, not point/click. 

<p> Some
users consider Gri similar to LaTeX, since both provide extensive
power as a reward for tolerating a learning curve.

<p>
Gri can make 
<a href="./gridoc/html/X-Y.html">
x-y
</a> graphs,
<a href="./gridoc/html/ContourPlots.html">
contour
</a> graphs, and 
<a href="./gridoc/html/Images.html">
image
</a>
graphs, in PostScript and (soon) SVG formats.
Control is provided over all aspects of drawing,
e.g. line widths, colors, and fonts.  A TeX-like syntax provides 
common mathematical symbols.

<h1>Starting to use Gri</h1>

<p>Folks who write thousand-line Gri scripts usually start with something
as simple as the following, which produces an auto-scaled graph.
<pre>
<font color="#82140F">
   open file.dat
   read columns x y
   draw curve
</font>

</pre>


<?php footer(); ?>