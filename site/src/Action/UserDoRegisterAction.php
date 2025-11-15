<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Odan\Session\SessionInterface;
use App\Backend\AdelphosBE;
use Cake\Validation\Validator;
use App\Module\Validation\Exception\ValidationException;
use Symfony\Component\Mime\Address;
use Symfony\Component\Mime\Email;
use Symfony\Component\Mailer\MailerInterface;
use App\Module\Mail\Service\Mailer;

final class UserDoRegisterAction
{

    public function __construct(
	    private PhpRenderer $renderer,
	    private SessionInterface $session,
	    private AdelphosBE $backend,
	    private Mailer $mailer

    ) {
    
	    // OK, this is the constructor.
    
    
    }

    public function __invoke_new(Request $request, Response $response): Response
    {
	    //$name = $data['name'] ?? 'Guest';

	    $params = (array)$request->getParsedBody();

	    $email = $params['email'] ?? 'what?';

	    $responseData = ['message' => "Hello, your email is $email"];


	    $response->getBody()->write((string)json_encode($responseData, 
		    JSON_UNESCAPED_SLASHES | JSON_PARTIAL_OUTPUT_ON_ERROR));
	    $response = $response->withStatus(200);
	    

	    return $response->withHeader('Content-Type', 'application/json');



	    //return $response->withJson($responseData); // Return JSON response


    }

    public function __invoke(Request $request, Response $response): Response
    {

	    $params = (array)$request->getParsedBody();
	    $email = $params['email'] ?? 'what?';

	    
	    $validator = new Validator();

	    $validator->email('email', false, "Invalid Email");

	    $errors = $validator->validate($params);

	    if (count($errors) > 0) {
		    // This is the exit point when there are errors.
		    throw new ValidationException($errors);
	    }

	   
	    $responseData = ['message' => "Hello, your email is $email"];

	    $response->getBody()->write((string)json_encode($responseData, 
		    JSON_UNESCAPED_SLASHES | JSON_PARTIAL_OUTPUT_ON_ERROR));
	    $response = $response->withStatus(200);
	    

	    return $response->withHeader('Content-Type', 'application/json');





    }

    public function __invoke_BAD(Request $request, Response $response): Response
    {

	    $params = (array)$request->getParsedBody();


	    $validator = new Validator();

	    $validator->email('email', false, "Invalid Email");

	    $errors = $validator->validate($params);

	    if ($errors) {
		    throw new ValidationException($errors);
	    }

	    // Create email object
	    $email = new Email();
	    // Set sender and reply-to address
	    $email->from(new Address('adelphos@adelphos.it', 'Sender Name'));

	    // Set subject
	    $email->subject('Welcome to adelphos');

	    // Get body HTML from template password-reset.email.php
	    $body = $this->mailer->getContentFromTemplate(
		    'mails/register_mail.html.php',
		    ['name' => 'lino']
	    );
	    // Set body
	    //$body = "<h1>hello</h1>";
	    $email->html($body);

	    // Add recipients and priority
	    $email->to(new Address($params['email'],  "nome cognome"))
	   ->priority(Email::PRIORITY_HIGH);

	    // Send email
	    //$this->mailer->send($email);




	    //////////////////////////////////////////////////

	    $bread_crumbs = [
		    'Home' => '/',
		    'name/family' => '/user/register',
		    'Enter code' => '/user/do_register_step_2'
	    ];


	    $attributes = [
		    'help_page' => 'create_user_step_two',
		    'bread_crumbs' => $bread_crumbs,
		    'params' => $params,
		    'errors' => $errors
	    ];
	    
            $attributes = make_bag_parameters($attributes, $this->session);

	    return $this->renderer->render($response, 'user/registration_wait_code.html.php', $attributes);

    }

}

?>
