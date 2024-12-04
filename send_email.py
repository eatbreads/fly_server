import smtplib
import ssl
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import sys

def send_email(from_email, to_email, subject, body, auth_code):
    smtp_server = "smtp.qq.com"
    smtp_port = 587

    # 创建 MIME 邮件
    message = MIMEMultipart()
    message["From"] = from_email
    message["To"] = to_email
    message["Subject"] = subject
    message.attach(MIMEText(body, "plain"))

    context = ssl.create_default_context()

    try:
        server = smtplib.SMTP(smtp_server, smtp_port)
        server.starttls(context=context)
        server.login(from_email, auth_code)

        server.sendmail(from_email, to_email, message.as_string())
        print("Email sent successfully!")

    except Exception as e:
        print(f"Email sending failed: {e}")

    finally:
        server.quit()

if __name__ == "__main__":
    # 从命令行参数获取邮件相关信息
    if len(sys.argv) != 6:
        print("Usage: python send_email.py <from_email> <to_email> <subject> <body> <auth_code>")
        sys.exit(1)

    from_email = sys.argv[1]  # 发件人邮箱
    to_email = sys.argv[2]    # 收件人邮箱
    subject = sys.argv[3]     # 邮件主题
    body = sys.argv[4]        # 邮件正文
    auth_code = sys.argv[5]   # QQ 邮箱的授权码

    send_email(from_email, to_email, subject, body, auth_code)
