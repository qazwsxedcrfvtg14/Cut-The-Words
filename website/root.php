<?php
	header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
	header("Cache-Control: post-check=0, pre-check=0", false);
	header("Pragma: no-cache");
	if($_POST["post"]!=""){
		$handle=fopen("root.txt","w");
		fwrite($handle,$_POST["post"]);
	}
	else{
		$handle=fopen("root.txt","r");
		echo fread($handle,100000000);
	}
?>