In this app we will send an email through JSON with the help of Mail API [SendGrid](https://app.sendgrid.com/)   
     
Go to Settings > API Keys > Full access    

As described in the documentation, you can make POST request to **url** by setting few **headers** and json **body**      
     
**URL**     
https://api.sendgrid.com/v3/mail/send     
      
**Headers**       
Authorization:  Bearer [YOU API KEYS HERE]       
Content-type:   application/json    

**Body**     
```json
{
  "personalizations": [
    {
      "to": [
        {
          "email": "asim.ibn.asghar@gmail.com",
          "name": "Bro"
        }
      ],
      "subject": "Hello, World!"
    }
  ],
  "content": [
    {
      "type": "text/plain",
      "value": "Heya!"
    }
  ],
  "from": {
    "email": "o0teboy@hotmail.com",
    "name": "Ibn"
  },
  "reply_to": {
    "email": "o0teboy@hotmail.com",
    "name": "Ibn"
  }
}

```     

You also need to download the certificate by typing in the browser https://api.sendgrid.com/v3/mail/send              
    

