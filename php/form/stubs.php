<?php

function randString($length = 8) {
	$str = '';
	for ($i = 1; $i <= $length; $i++) { 
		$d = rand(1,30)%2; 
		$str .= $d ? chr(rand(65,90)) : chr(rand(48,57)); 
	} 
	return $str;
}

class results {
	public $id;
	public $p1;
	public $p2;
	public $email;
	public $add;

	function __construct() {
		$this->id = rand(0,100);
		$this->p1 = randString();
		$this->p2 = randString();
		$this->email = randString(4)."@example.com";
		$this->add = randString(20);
	}
}

function db_query($query) {
	return true;
}

function db_fetch_object($result) {
	static $rows = 0;

	if ($rows < 10) {
		$rows++;
		return new results();
	} else {
		return false;
	}
}

?>
