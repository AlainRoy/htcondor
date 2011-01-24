<html>
<body>
<title>NWO task results</title>

<?php
include "last.inc";
require_once "./load_config.inc";                                     
load_config(); 

$platform    = $_REQUEST["platform"];
$task        = $_REQUEST["task"];
$description = $_REQUEST["description"];
$runid       = $_REQUEST["runid"];

# Some CSS and Javascript
echo '
<style type="text/css">
<!--
pre {
   margin-left: 1em;
   margin-right: 1em;
   padding: 0.5em;
   width: 90%;
   background-color: #f0f0f0;
   border: 1px solid black;
   white-space: pre-wrap;       /* css-3 */
   white-space: -moz-pre-wrap !important; /* Mozilla + Firefox */
   white-space: -pre-wrap;      /* Opera 4-6 */
   white-space: -o-pre-wrap;    /* Opera 7 */
   word-wrap: break-word;       /* Internet Explorer 5.5+ */
}

font.hl {
   BACKGROUND-COLOR: yellow;
}
-->
</style>

<script type="text/javascript" language="JavaScript">
<!--
function swap(d) {
    if(document.getElementById(d).style.display == "none") { 
        document.getElementById(d).style.display = "block"; 
    }
    else { 
        document.getElementById(d).style.display = "none"; 
    }
}
-->
</script>
';

echo "<P>";
echo "<B>Tag: </b>$description<BR>";
echo "<B>Task: </b>$task<br>";
echo "<B>Platform: </b>$platform<br>";
echo "</P>";

$query = "
SELECT
  Task.name,
  convert_tz(Task.start, 'GMT', 'US/Central') as start,
  convert_tz(Task.finish, 'GMT', 'US/Central') as finish,
  timediff(Task.finish, Task.start) as duration,
  Task.result,
  Run.host,
  Run.gid,
  Run.runid,
  Run.filepath,
  Task.task_id
FROM
  Task LEFT JOIN Run USING (runid)
WHERE 
  Task.name='".$task."' AND
  Task.platform='".$platform."'
";

if ($runid) {
  $query.=" AND Task.runid=$runid";
}
$query .= "
ORDER BY
  Task.start DESC
";


$db = mysql_connect(WEB_DB_HOST, DB_READER_USER, DB_READER_PASS) 
      or die ("Could not connect : " . mysql_error());
mysql_select_db(DB_NAME) or die("Could not select database");

$result = mysql_query($query) or die ("Query failed : " . mysql_error());

while( $myrow = mysql_fetch_array($result) ) {
  $taskname=$myrow["name"];
  $hostname=$myrow["host"];
  $gid=$myrow["gid"];
  $base_filepath=$myrow["filepath"];
  #echo "<h1>$hostname/$base_filepath/$gid/$taskname</h1>";

  # use .out file to determine path - .err file base path is indentical  
  $filepath = "";
  $basedir = $base_filepath; //"rundir";   
  //$local_fs_prefix = "/nmi/nwo/www/html/";

  if ( strstr($taskname, "platform_job") ){
    $filepath = "$basedir/$gid/$taskname.$platform";
    $file_found=file_exists($filepath.".out");;
    #echo "<h1>$filepath</h1>";
  }
  else if ( strstr($taskname, "platform_post") ){
    $filepath = "$basedir/$gid/$taskname.$platform";
    $file_found=file_exists($filepath.".out");;
    #echo "<h1>$filepath</h1>";
  }
  else if ( strstr($taskname, "platform_pre") ){
    $filepath = "$basedir/$gid/$taskname.$platform";
    $file_found=file_exists($filepath.".out");;
    #echo "<h1>$filepath</h1>";
  }
  else if (strstr($platform, "local"))  {   
    $filepath = "$basedir/$gid/$taskname";
    $file_found = file_exists($filepath.".out");
    #echo "<h1>$filepath</h1>";
  }
  else {   
    $filepath = "$basedir/$gid/userdir/$platform/$taskname";
    $file_found = file_exists($filepath.".out");
    #echo "<h1>$filepath</h1>";
  }

  if (!$file_found) {
    $stdout_url = "N/A";
    $stderr_url = "N/A";
  }
  else {
    $stat = stat("$filepath.out");
    $stdout_size = $stat['size'];
    
    $stat = stat("$filepath.err");
    $stderr_size = $stat['size'];
    
    $stdout_url = "<a href=\"http://$hostname/$filepath.out\">".basename($filepath).".out</a>";
    $stderr_url = "<a href=\"http://$hostname/$filepath.err\">".basename($filepath).".err</a>";
  }
  
  $resultspath = "$basedir/$gid/userdir/$platform/results.tar.gz";
  $resfound = file_exists($local_fs_prefix.$resultspath);
  
  if(!$resfound) {
    $results_url = "N/A";
  }
  else {
    $results_url = "<a href=\"http://$hostname/$resultspath\">".basename($resultspath)."</a>";
  }
  
  $resultval = $myrow["result"];
  $test_results_url = "<a href=\"http://nmi.cs.wisc.edu/node/552\">".$resultval."</a>";
  echo "<P>";
  echo "<TABLE>";
#    echo "<TR><TD>Run ID:</TD><TD><a href=\"http://$hostname/nmi/?page=results/runDetails&id=".$myrow["runid"] ."\">".$myrow["runid"]."</a></TD></TR>";
    #echo "<TR><TD>Run ID:</TD><TD><a href=\"http://$hostname/nmi/?page=results/runDetails&runid=".$myrow["runid"] 
  echo "<TR><TD>Run ID:</TD><TD><a href=\"./Task-search.php?runid=".$myrow["runid"]."&Submit=Search+by+RunID" 
    ."\">".$myrow["runid"]."</a></TD></TR>";
  echo "<TR><TD>GID:</TD><TD>".$myrow["gid"] ."</TD></TR>";
  echo "<TR><TD>Task ID:</TD><TD>".$myrow["task_id"] ."</TD></TR>";
  echo "<TR><TD>Start:</TD><TD>".$myrow["start"] ."</TD></TR>";
  echo "<TR><TD>Finish:</TD><TD> ".$myrow["finish"] ."</TD></TR>";
  echo "<TR><TD>Duration:</TD><TD> ".$myrow["duration"] ."</TD></TR>";
  echo "<TR><TD>Result:</TD><TD> $test_results_url </TD></TR>";
  echo "<TR><TD>Stdout:</TD><TD> $stdout_url - (size: $stdout_size bytes) </TD></TR>";
  echo "<TR><TD>Stderr:</TD><TD> $stderr_url - (size: $stderr_size bytes) </TD></TR>";
  echo "<TR><TD>Run Results:</TD><TD> $results_url</a></TD></TR>";
  echo "</TABLE>";
  echo "</P>";

  if($file_found) {
    if($stdout_size > 0) {
      show_file_content("STDOUT", "$filepath.out");
    }
    
    if($stderr_size > 0) {
      show_file_content("STDERR", "$filepath.err");
    }
  }
}

mysql_free_result($result);
mysql_close($db);

function show_file_content($header, $file) {
  echo "<hr>\n";
  echo "<h3>$header:</h3>";
  
  $lines = `grep -C 5 -i error $file`;
  echo "<p style=\"font-size: 80%;\">Showing all instances of the word 'error' in $header:\n";
  if(strlen($lines) > 0) {
    $lines = preg_replace("/(error)/i", "<font class=\"hl\">$1</font>", $lines);
    echo "<p><a href=\"javascript:swap('$header')\">Click to show errors in $header</a>\n";
    echo "<div id=\"$header\" style=\"display:none;\">\n";
    echo "<pre>$lines</pre>\n";
    echo "</div>\n";
  }
  else {
    echo "<p>The string 'error' was not present in $header\n";
  }

  // Always show the last 10 lines
  $count = 10;
  $lines = `tail -$count $file`;
  echo "<p style=\"font-size: 80%;\">Last $count lines of $header:\n";
  echo "<pre>$lines</pre>";
}

?>
</body>
</html>

