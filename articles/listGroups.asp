<%@ language = JScript %>
<h1>User information</h1>
<%
var user=GetObject("WinNT://DOLPHIN/denish");
if (user != null) {
%>
<table>
<tr><td>Class:</td><td><%=user.Class%></td></tr>
<tr><td>Name:</td><td><%=user.Name%></td></tr>
<tr><td>Full Name:</td><td><%=user.FullName%></td></tr>
<tr><td>Description:</td><td><%=user.Description%></td></tr>
<tr><td>Last Login:</td><td><%=user.LastLogin%></td></tr>
<tr><td>Password Expiration date:</td><td><%=user.PasswordExpirationDate%></td></tr>
<tr><td>Account Disabled:</td><td><%=user.AccountDisabled%></td></tr>
<tr><td valign="top">Groups:</td><td>
<%
    for (var e = new Enumerator (user.Groups()); ! e.atEnd(); e.moveNext())
    {
        var oGroup = e.item();
        Response.write(oGroup.Name+"<br>\n");
    }
%></td></tr>
</table>
<%
} else {
    Response.write("User is null");
}
%>
</body>
</html>
