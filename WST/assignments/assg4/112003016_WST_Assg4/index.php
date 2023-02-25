<!DOCTYPE html>
<html>
    <head>
        <title>WST Assignment 4</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="icon" href="./favicon-32x32.png" type="image/png" />
        <link rel="stylesheet" href="./css/bootstrap.min.css" />
        <link rel="stylesheet" href="./css/wst-assg4.css" />
    </head>

    <body>
        <div class="w-navbar">
            <div class="container">WST Assignment 4</div>
        </div>
        <div class="container w-content">
            <div class="row">
                <div class="col-md-4">
                    <form action="submit">
                        <div class="form-group">
                            <label for="firstname">First Name</label>
                            <input
                                class="form-control"
                                type="text"
                                name="firstname"
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
                                id="username"
                                minlength="4"
                                required />
                        </div>
                        <div class="form-group">
                            <label for="email">Email ID</label>
                            <input
                                class="form-control"
                                type="email"
                                name="email"
                                id="email"
                                minlength="2"
                                pattern="[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,}$"
                                required />
                        </div>
                        <div class="form-group">
                            <label for="state">State</label>
                            <input
                                class="form-control"
                                type="text"
                                name="state"
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
                                minlength="2"
                                required />
                        </div>

                        <button class="btn btn-primary" type="submit">
                            Send
                        </button>
                        <button class="btn btn-primary" type="reset">
                            Clear
                        </button>
                    </form>
                </div>
            </div>
        </div>
    </body>
</html>
