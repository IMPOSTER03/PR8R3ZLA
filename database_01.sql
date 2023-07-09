create table Users (
    EmailID varchar(255) PRIMARY KEY,
    Username varchar(255) not NULL,
    PasswordHash varchar(255) not NULL,
    OTPCode varchar(6),
    OTPExpiration DATETIME
);

create table PasswordResetRequests (
    RequestID int PRIMARY KEY,
    EmailID varchar(255),
    RequestToken varchar(255) not NULL,
    RequestExpiration DATETIME,
    foreign key (EmailID) references Users(EmailID)
);

create table OTPLoginRequests (
    RequestID int PRIMARY KEY,
    EmailID varchar(255),
    RequestToken varchar(255) not NULL,
    RequestExpiration DATETIME,
    foreign key (EmailID) references Users(EmailID)
);

DELIMITER //
create procedure VerifyPassword(
    in p_Username varchar(255),
    in p_Password varchar(255),
    out p_EmailID varchar(255),
    out p_Result BOOLEAN
)
begin
    declare p_HashedPassword varchar(255);
    select EmailID, PasswordHash into p_EmailID, p_HashedPassword from Users where Username = p_Username;
    set p_Result = binary bcrypt_checkpw(p_Password, p_HashedPassword);
end //

DELIMITER ;

DELIMITER //
create procedure VerifyOTPCode(
    in p_EmailID varchar(255),
    in p_OTPCode varchar(6),
    out p_Result BOOLEAN
)
begin
    declare p_CurrentDateTime DATETIME;
    declare p_OTPExpiration DATETIME;
    select current_timestamp, OTPExpiration into p_CurrentDateTime, p_OTPExpiration from Users where EmailID = p_EmailID;
    if p_CurrentDateTime <= p_OTPExpiration and p_OTPCode = OTPCode then
        set p_Result = TRUE;
    else
        set p_Result = FALSE;
    end if;
end //
DELIMITER ;
