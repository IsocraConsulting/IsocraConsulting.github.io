---
title: Generating a database dump SQL Script from Java
author: denish
layout: post
permalink: /2004/10/dumptosql/
categories:
  - Articles
  - Java
  - Persistence
tags:
  - databases
  - Java
  - SQL
---
The aim of this project is to generate an SQL script from an existing database such that the tables and data can be recreated exactly as it was. <!--more-->There are lots of reasons for having such a tool, for example:

  * We want to transfer the development database from our machine to the client&#8217;s server
  * We&#8217;re developing on different sites so we need to be able to duplicate database schemas and data
  * We&#8217;re developing using one database type (SQLServer, Oracle, MySQL&#8230;) and we need to test and/or deploy on another
  * We want to be able to keep snapshots of the database schema and data in a version control system such as CVS in a form where we can easily compare versions

Most database vendors provide a way of executing SQL scripts but often, exporting both the schema and data from a database as SQL can be difficult (SQLServer for example allows you to export the schema, but not—as far as we know—the data). The other problem though is that each database vendor understands SQL slightly differently which makes making a vendor-independent SQL file beyond this project. Instead, the code presented here will generate an SQL file which you may then have to tweak to get it to run on your database.

## Background

Fortunately, Java&#8217;s JDBC interface provides most of the functionality that we need to be able to interrogate a database and extract the tables and columns and types. It&#8217;s then simply a matter of calling `SELECT * FROM <em><tablename></em>` to extract the data. We can then generate the appropriate `CREATE TABLE ...` and `INSERT INTO ...` statements to get our script.

The first thing we need to do is to connect to the database. We&#8217;ve used `Class.forName(<em><driverClassName></em>)` and `DriverManager.getConnection(<em><driverURL></em>, <em><properties></em>)` to load up the appropriate JDBC driver and get a connection. We can then call `connection.getMetaData()` to get the database metadata. From here, we can get the tables and then for each table get the columns, the primary keys and then the data. So far we haven&#8217;t bothered to get any other constraints, so these would have to be added by hand.

In order to make the code as easy to use as possible, we&#8217;ve used a properties file to contain all the configuration information, so the username, password, driverURL, driverClass, schema pattern, tableName pattern, catalog and table name quote character are all read from the properties file. To use the code, you will need to create a properties file where you fill in the values as described below:

```properties
# Properties file for controlling db2sql.java

# Driver information (make sure the appropriate classes/jars are on the classpath)
# ==================
#
# These are mandatory, you must provide appropriate values
driver.url=jdbc:mysql://localhost/testdb
driver.class=com.mysql.jdbc.Driver

# Information passed to DriverManger.getConnection
# ================================================
#
# Put any information here that you want to pass to the DriverManager, for example:
user=&lt;username&gt;
password=&lt;password&gt;

# Configuration information
# =========================
#
# Optional information that you can set to control which tables are output etc.
# See <a href="http://java.sun.com/j2se/1.4.2/docs/api/java/sql/DatabaseMetaData.html#getTables%28java.lang.String,%20java.lang.String,%20java.lang.String,%20java.lang.String%5B%5D%29">
</a>target="_BLANK"&gt;DatabaseMetaData.getTables(...) for information on how to use these.
# Leave these blank to get all tables (note that only normal tables are returned, not
# views, system tables, temporary tables, synonyms or aliases)
# catalog=
# schemaPattern=
# tableName=
#
# You can also specify a quote character that is used to surround column names. This is
# useful if your tables contain any SQL-unfriendly characters such as hyphens. This is
# configurable so that you can set it up for your target database rather than the source
# database. Note however that if you do specify a quote character then the case of the
# table will probably become significant and this may not be what you want. Note also
# that this is <strong>not</strong> the same as the quote character for data values. For that a single
# quote is used always. Leave this blank for no quote character.
# columnName.quoteChar="
```

## Notes

<ol class="wide">
  <li>
    Once you&#8217;ve got the script, you&#8217;ll need to be able to execute it against your database. In the end, we wrote a JSP which would upload the script and then execute because we were using JSPs anyway and they had an easy way to get a connection to the current database. However, in order to use <code>statement.execute()</code> we first had to tokenise the SQL file using the semi-colon to spot the individual statements and then we executed each statement<br /> one after the other. The only problem with this is that any backslashes in the file are interpreted rather than just left as they are, so if you intend to do something like this, you need to first go through and quote the backslashes.
  </li>
  <li>
    We had a database that had values which included the ampersand sign (&) [there were snippets of HTML in the data]. We were trying to import these into Oracle using SQLPlus. Unfortunately, SQLPlus interprets the ampersand as a variable and assumes that the next line is the value which screws everything up. We found out (from this rather excellent site: <a href="http://www.orafaq.org/faqplus.htm">Oracle SQL*Plus FAQ</a>) that adding<br /> <code>SET DEFINE OFF</code> stops SQLPlus from prompting for variable names.
  </li>
  <li>
    The script works OK for all the basic types, but isn&#8217;t going to work for large types like BLOBS, CLOBS, MEMO (in access) LONG VARCHAR and all sorts of unusual column types. Watch out for this and alter the script accordingly.
  </li>
  <li>
    If you are trying to suck database information out of Microsoft Access, then you&#8217;ll get errors saying that it can&#8217;t get the primary keys for the table. This is a limitation it seems either with the ODBC manager or with the JDBC-ODBC link. So again, you&#8217;ll have to add these by hand afterwards.
  </li>
</ol>

## Download Source

Download [the source][1].

 [1]: /articles/db2sql.java
