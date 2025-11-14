<?php

namespace App\Backend\sockBE;

use App\Backend\AdelphosBE;
use App\Data\CurrencyCollection;
use App\Data\UserData;

/*
 *
 * The backend which uses a socket.
 *
 * The backend is initializes as soon the page is called.
 *
 */
class SocketBE implements AdelphosBE {

	private $socket;
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

	function add_user(UserData $user){

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

		$myhello = "42 the meaning of everything";
		$res = socket_write($this->socket, $myhello);
		if ($res === false) {
			goto throw_error;
		}

		$this->daemon_answer = socket_read($this->socket, 200);
		if ($this->daemon_answer === false) {
			goto throw_error;
		}

		// all good
		return;


		throw_error:
		throw new \Exception(socket_strerror(socket_last_error()));
		
	}

}


?>
