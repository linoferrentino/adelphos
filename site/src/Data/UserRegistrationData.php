<?php


namespace App\Data;


/*
 *
 * A user in adelphos.
 *
 * This is used when we want to create a user.
 *
 * in this case the user does not have yet an id.
 *
 *
 */
readonly class UserRegistrationData {

	public function __construct(
		public string $name,
		public string $family,
		public string $email,
		public string $password)
	{
	}


}



?>
