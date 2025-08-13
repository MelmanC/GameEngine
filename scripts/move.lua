local script = {}

SPACE = 32
W = 87
A = 65
S = 83
D = 68

G = 71
H = 72
J = 74
Y = 89

function script.OnStart(entity)
	print("Cube script started.")
	print("Entity name: " .. Entity.getName(entity))
end

function script.OnUpdate(entity, deltaTime)
	local pos = Transform.getPosition(entity)
	local moveSpeed = 3 * deltaTime
	local jumpSpeed = 5 * deltaTime

	if Input.isKeyPressed(Y) then
		pos.x = pos.x + moveSpeed
	elseif Input.isKeyPressed(H) then
		pos.x = pos.x - moveSpeed
	end
	if Input.isKeyPressed(G) then
		pos.z = pos.z - moveSpeed
	elseif Input.isKeyPressed(J) then
		pos.z = pos.z + moveSpeed
	end
	if Input.isKeyPressed(SPACE) then
		print("Space key pressed!")
		pos.y = pos.y + jumpSpeed
	end

	Transform.setPosition(entity, pos.x, pos.y, pos.z)
end

function script.OnDestroy(entity)
	print("Cube script destroyed.")
end

return script
