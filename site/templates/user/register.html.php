<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Step 1, yourself and your family (help is available, click above) </h3>
<!-- <p> <x?= $bag['daemon_answer'] ?> </p> -->


    <form action="/user/do_register"
          id="reg_step_1" class="form-container" method="post" autocomplete="off">

<div class="form-row">
            <label class="form-label" for="userid">User name:</label>
            <input class="form-input" name="userid" type="text" id="userid">

</div>
<div class="form-row">
            <label class="form-label" for"family">Family name:</label>
            <input class="form-input"  id="family" type="text" name="family">
</div>
<div class="form-row">
            <label class="form-label" for"email">Email:</label>
            <input class="form-input"  id="email" type="text" name="email">
</div>
<div class="form-row">
            <label class="form-label" for"password">Password:</label>
            <input class="form-input"  id="password_1" type="password" name="password">
</div>

<div class="form-row">
            <label class="form-label" for"password">Confirm password:</label>
            <input class="form-input"  id="password_2" type="password" name="password_2">
</div>
<div class="form-row">
            <button type="submit" class='form-submit'>Next...</button>
</div>
    </form>

<script>
$(document).ready(function () {
   $('#reg_step_1').submit(function (e) {
       e.preventDefault(); // Prevent default form submission
       $.ajax({
           url: '/user/do_register',
           type: 'POST',
           data: { email: $('#email').val() }, // Send form data
           success: function (response) {
               console.log(response.message); // Handle success
               alert("Success:" +  response.message);
           },

	   error: function(xhr, status, error) {
		   console.log(xhr.responseText)
		   //var errorMessage = xhr.responseText ? JSON.parse(xhr.responseText).message : "An error occurred";
		   var errorMessage = xhr.responseText ? JSON.parse(xhr.responseText).data['errors']['email'][0] : "An error occurred";
		   document.getElementById('email').style.backgroundColor = 'red';
		   alert("Please correct the errors " + errorMessage);
	   }
           
       });
   });
});


</script>
