<?php

namespace App\Backend\sockBE;

use App\Backend\AdelphosBE;
use App\Data\CurrencyCollection;
use App\Data\UserData;
use App\Data\UserRegistrationData;

/*
 *
 * The backend which uses a socket.
 *
 * The backend is initializes as soon the page is called.
 *
 */
class SocketBE implements AdelphosBE {

	private $socket = null;
	public $daemon_answer;


	// in the constructor I will open the socket.
	function __construct() {

		$this->open_adelphos_socket();

	}


	function echo_me(string $text) : string {

		return $this->daemon_answer;

	}

	// the mock adelphos must return a cursor to the "fake" table of the
	// currencies.
	function get_currencies() : CurrencyCollection {

		return new CurrencyCollection();
	}

	/*
	function add_user(UserData $user){

	}
	 */


	private function _socket_write_all($str, $len) {

		$res = socket_write($this->socket, $str, $len);

		if ($res === false) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}

		// here I have to make a loop
		if ($res < $len) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}

	}


	private function _socket_read_all($len) {

		$str = socket_read($this->socket, $len);

		if ($this->daemon_answer === false) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}
		// here I have to make a loop
		if (strlen($str) < $len) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}

		return $str;

	}



	private function _socket_write_request($request) {

		$len = strlen($request);

		$packed = pack("L", $len);

		// this string is exactly 4 bytes
		$this->_socket_write_all($packed, 4);

		$this->_socket_write_all($request, $len);
			
	}

	private function _socket_read_response() {
		// first of all I read 4 bytes,

		$lens = $this->_socket_read_all(4);

		$len_i = unpack("L", $lens)[1];

		$response = $this->_socket_read_all($len_i);

		return $response;

	}
	
	function add_user(UserRegistrationData $user_data)
	{
		// let's encode the data.
		$request = json_encode($user_data);

		// the length of the string is stored.

		/*
		$res = socket_write($this->socket, $request);
		if ($res === false) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}
		 */
		$this->_socket_write_request($request);

		/*
		$this->daemon_answer = socket_read($this->socket, 200);
		if ($this->daemon_answer === false) {
			throw new \Exception(socket_strerror(socket_last_error()));
		}
		 */
		$this->daemon_answer = $this->_socket_read_response();

		return $this->daemon_answer;

	}

	public function open_adelphos_socket()
	{

		$this->socket = socket_create(AF_UNIX, SOCK_STREAM, 0);
		if ($this->socket === false) {
			goto throw_error;
		}

		$res = socket_connect($this->socket, "/tmp/adelphos");
		if ($res === false) {
			goto throw_error;
		}

		// Ok, now we send some data. 

	/*	
		$myhello = "42 the meaning of everything";
		$res = socket_write($this->socket, $myhello);
		if ($res === false) {
			goto throw_error;
		}

		$this->daemon_answer = socket_read($this->socket, 200);
		if ($this->daemon_answer === false) {
			goto throw_error;
		}
	 */
		

		// all good
		return;


		throw_error:
		throw new \Exception(socket_strerror(socket_last_error()));
		
	}

}


?>
