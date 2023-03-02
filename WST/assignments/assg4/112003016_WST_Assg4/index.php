<!DOCTYPE html>
<html>
    <head>
        <title>WST Assignment 4 - Contact Manager</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="stylesheet" href="./css/bootstrap.min.css" />
        <link rel="stylesheet" href="./css/wst-assg4.css" />
    </head>

    <body>

    <?php

        include('database.php');
         
        $db = $conn;
        $data = $_POST;
     
        if( $_SERVER["REQUEST_METHOD"]  == "POST")
        {           
            // fetch record 
            $recorduser = get_contact_for_username($db, $_POST['username']);
            $recordemail = get_contact_for_email($db,  $_POST['email']);
            if( is_array($recorduser)){
                $username_error = "Contact with username: ". $_POST['username'] . " already exits in database, please chose new username ";
                $sessdata = $_POST;
            }
            else if( is_array($recordemail))
            {
                $email_error = "Contact with email: " . $_POST['email'] . " already exits in database, please chose new email";  
                $sessdata = $_POST;
            }
            else{
                $msg = add_contact( $db, $_POST);
            }
        }

    ?>
        <div class="w-navbar">
            <div class="container">
                <div class="btn btn-xs btn-light " style="float:left" class="m-2" ><a href="/index.php">Add Contact</a></div>
                <div class="btn btn-xs btn-light ml-2" style="float:left" class=" ml-2 m-2"><a href="/list.php">List Contacts</a></div>
                
                <div style="float:right" class="m-2">WST Assignment 4</div>
            </div>
        </div>
        <div class="container w-content">
            <div class="row">
                <div class="col-md-4">
                    <p class="text-success"> <?php echo $msg??''; ?> </p>
                    <form id="contact-form" action="index.php" method="POST">
                        <div class="form-group">
                            <label for="firstname">First Name</label>
                            <input
                                class="form-control"
                                type="text"
                                name="firstname"
                                value="<?php echo $sessdata['firstname']??''; ?>"
                                id="firstname"
                                minlength="2"
                                required />
                        </div>
                        <div class="form-group">
                            <label for="lastname">Last Name</label>
                            <input
                                class="form-control"
                                type="text"
                                name="lastname"
                                value="<?php echo $sessdata['lastname']??''; ?>"
                                id="lastname"
                                minlength="2"
                                required />
                        </div>
                        <div class="form-group">
                            <label for="username">Username</label>
                            <input
                                class="form-control"
                                type="text"
                                name="username"
                                value="<?php echo $sessdata['username']??''; ?>"
                                id="username"
                                minlength="4"
                                required />
                            <p class="text-danger"> <?php echo $username_error??''; ?> </p>
                        </div>
                        <div class="form-group">
                            <label for="email">Email ID</label>
                            <input
                                class="form-control"
                                type="email"
                                name="email"
                                value="<?php echo $sessdata['email']??''; ?>"
                                id="email"
                                minlength="2"
                                pattern="[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,}$"
                                required />
                            <p class="text-danger"><?php echo $email_error??''; ?> </p>
                        </div>
                        <div class="form-group">
                            <label for="state">State</label>
                            <input
                                class="form-control"
                                type="text"
                                name="state"
                                value="<?php echo $sessdata['state']??''; ?>"
                                id="state"
                                minlength="2"
                                required />
                        </div>
                        <div class="form-group">
                            <label for="country">Country</label>
                            <input
                                class="form-control"
                                type="text"
                                name="country"
                                id="country"
                                value="<?php echo $sessdata['country']??''; ?>"
                                minlength="2"
                                required />
                        </div>

                        <button class="btn btn-primary" type="submit">
                            Send
                        </button>
                        <button class="btn btn-danger" type="button" onclick="clearform()">
                            Clear
                        </button>
                    </form>
                </div>
            </div>
        </div>
        <script src="./js/jquery.min.js"></script>
        <script src="./js/bootstrap.min.js"></script>
        <script src="./js/wst-assg4.js"></script>
    </body>
</html>
