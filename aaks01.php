<?php

$a = [];
$fp = fopen($argv[1], "r") or die("read");
while ($l = fgets($fp))
{
	$l = explode(",", $l);
	$a[(int)$l[1]] = trim($l[0]);
}
ksort($a);
$fp = fopen($argv[2], "w") or die("write");
$n = 0;
foreach ($a as $k => $v)
{
	fwrite($fp, "$v, $k\n");
	if (++$n == 100)
		break;
}
fclose($fp);
