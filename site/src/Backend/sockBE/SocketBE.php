<?php

namespace App\Backend\sockBE;

use App\Backend\AdelphosBE;
use App\Data\CurrencyCollection;
use App\Data\UserData;
use App\Data\UserRegistrationData;
use Odan\Session\SessionInterface;

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
	// I need the session because I will get the authorization token.
	function __construct(
	    private SessionInterface $session
	) {

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


	private function _socket_write_all($str, $len) {

		$len_written = 0;

		while ($len_written < $len) {

			$wrb = socket_write($this->socket, $str, $len);

			if ($wrb === false) {
				throw new \Exception(socket_strerror(socket_last_error()));
			}

			$len_written += $wrb;

		}

	}


	private function _socket_read_all($len) {

		$len_read = 0;

		while ($len_read < $len) {

			$str = socket_read($this->socket, $len);
			if ($str === false) {
				throw new \Exception(socket_strerror(socket_last_error()));
			}

			if ($len_read == 0) {
				$str_tot = $str;
			} else {
				$str_tot = $str_tot . $str;
			}

			$len_read += strlen($str);

		}

		return $str_tot;

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


	
	// This is the function to create a request.
	// Not all the requests needs an authorization
	protected function _do_request($cmd, $needs_auth, $data) {

		$request = [
			"cmd" => $cmd,
			"params" => $data
		];

		if ($needs_auth === true) {
			$request['token'] = $this->session['token'];
		}
	
		$request_json = json_encode($request, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES, JSON_NUMERIC_CHECK);

		$this->_socket_write_request($request_json);

		$this->daemon_answer = $this->_socket_read_response();

		return $this->daemon_answer;

	}

	
	function add_user(UserRegistrationData $user_data)
	{
		return $this->_do_request('add_user', false, $user_data);
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


		// all good
		return;


		throw_error:
		throw new \Exception(socket_strerror(socket_last_error()));
		
	}

}


?>
