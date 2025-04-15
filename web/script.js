let currentFile = '';

function loadFiles() {
  fetch('/files')
    .then(response => response.json())
    .then(files => {
      const fileList = document.getElementById('fileList');
      fileList.innerHTML = '';
      files.forEach(file => {
        const li = document.createElement('li');
        li.textContent = file;
        li.onclick = () => viewFile(file);
        fileList.appendChild(li);
      });
    })
    .catch(err => alert('Error loading files: ' + err));
}

function saveFile() {
  const content = document.getElementById('content').value;
  if (!content.trim()) {
    alert('Please enter some text');
    return;
  }
  fetch('/save', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'content=' + encodeURIComponent(content)
  })
    .then(response => response.text())
    .then(data => {
      alert(data);
      document.getElementById('content').value = '';
      loadFiles();
    })
    .catch(err => alert('Error saving file: ' + err));
}

function viewFile(filename) {
  currentFile = filename;
  fetch('/file?name=' + encodeURIComponent(filename))
    .then(response => {
      if (!response.ok) throw new Error('File not found');
      return response.text();
    })
    .then(content => {
      const fileView = document.getElementById('fileView');
      document.getElementById('currentFile').textContent = filename;
      document.getElementById('fileContent').value = content;
      document.getElementById('fileContent').setAttribute('readonly', 'true');
      document.getElementById('saveEdit').style.display = 'none';
      fileView.style.display = 'block';
    })
    .catch(err => alert('Error loading file: ' + err));
}

function copyContent() {
  const content = document.getElementById('fileContent').value;
  navigator.clipboard.writeText(content)
    .then(() => alert('Content copied to clipboard'))
    .catch(err => alert('Error copying: ' + err));
}

function enableEdit() {
  const textarea = document.getElementById('fileContent');
  textarea.removeAttribute('readonly');
  document.getElementById('saveEdit').style.display = 'inline-block';
}

function saveEdit() {
  const content = document.getElementById('fileContent').value;
  fetch('/edit', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'name=' + encodeURIComponent(currentFile) + '&content=' + encodeURIComponent(content)
  })
    .then(response => response.text())
    .then(data => {
      alert(data);
      document.getElementById('fileContent').setAttribute('readonly', 'true');
      document.getElementById('saveEdit').style.display = 'none';
    })
    .catch(err => alert('Error saving edit: ' + err));
}

function deleteFile() {
  if (!confirm('Are you sure you want to delete ' + currentFile + '?')) return;
  fetch('/delete', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: 'name=' + encodeURIComponent(currentFile)
  })
    .then(response => response.text())
    .then(data => {
      alert(data);
      document.getElementById('fileView').style.display = 'none';
      loadFiles();
    })
    .catch(err => alert('Error deleting file: ' + err));
}

function clearEditor() {
  document.getElementById('content').value = '';
}

// Load files on page load
document.addEventListener('DOMContentLoaded', loadFiles);
