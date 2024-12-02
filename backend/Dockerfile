FROM python:3.9
WORKDIR /code
COPY ./requirements.txt /code/requirements.txt
RUN pip install --no-cache-dir --upgrade -r /code/requirements.txt
COPY ./app /code/app
COPY wait-for-it.sh /code/wait-for-it.sh
RUN chmod +x /code/wait-for-it.sh
EXPOSE 80
CMD ["./wait-for-it.sh", "mysql-db:3306", "--", "uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "80", "--reload"]